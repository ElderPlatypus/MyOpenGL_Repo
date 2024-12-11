#pragma once

//Includes c++
#include <typeindex>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <random>
#include <stdexcept>

//Full Incluson
#include "../Physics/Collision.h"
#include "Components.h"

///_________________________System Interface Class___________________________
struct ISystem
{
	virtual void Update(const std::unordered_map<std::string, std::shared_ptr<Actor>>& sceneGraph) = 0;
	~ISystem() = default;
};

class SceneGraphSystem : public ISystem
{
private:
	 std::shared_ptr<ComponentArchive<SceneGraphComponent>> graphManager;  

public:
	explicit SceneGraphSystem(const std::shared_ptr<ComponentArchive<SceneGraphComponent>>& _graphManager)
		: graphManager(_graphManager) { }

	~SceneGraphSystem()
	{
		for (const auto& [name, actors] : graphManager->GetAllComponents())
		{
			for (const auto& [name, object] : actors->uActorMap)
			{
				object->~Actor(); 
			}
			for (const auto& [name, object] : actors->uBoundsMap) 
			{
				object->~Actor();
			}
			for (const auto& [name, object] : actors->uSpawnerMap)
			{
				object->~Spawner(); 
			}
		}
	}
	void Update(const std::unordered_map<std::string, std::shared_ptr<Actor>>& sceneGraph) override
	{
		for (const auto& [name, actor] : sceneGraph)
		{
			if (graphManager->HasComponent(name))
			{
				graphManager->AddComponent(name, std::make_shared<SceneGraphComponent>());
			}
		}
	} 
	void SetShader(const std::shared_ptr<Shader>& shader)
	{
		for (const auto& [name, actors] : graphManager->GetAllComponents())  
		{
			for (const auto& [name, object] : actors->uActorMap) 
			{
				object->SetShader(shader);
			}
			for (const auto& [name, object] : actors->uBoundsMap)
			{
				object->SetShader(shader); 
			}
			for (const auto& [name, object] : actors->uSpawnerMap)
			{
				object->SetShaderAll(shader); 
			}
		}
	}
	void Update(float dt)
	{
		for (const auto& [name, actors] : graphManager->GetAllComponents())
		{
		   for (const auto& [name, object] : actors->uActorMap)
		   {
		     object->UpdateActors(dt);
		   }
		   
		   for (const auto& [name, object] : actors->uBoundsMap)
		   {
		   	  object->UpdateActors(dt);
		   }
		   
		   for (const auto& [name, object] : actors->uSpawnerMap)
		   {
		   	  object->UpdateAll(dt);
		   }
		}
	}
	void SetTextureActors(TextureType textureType)
	{
		for (const auto& [name, actors] : graphManager->GetAllComponents())
		{
			for (const auto& [name, object] : actors->uActorMap)
			{
				object->UseTexConfig(true, textureType); 
			}
		}
	}
	void SetTextureBounds(TextureType textureType)
	{
		for (const auto& [name, actors] : graphManager->GetAllComponents())
		{
			for (const auto& [name, object] : actors->uBoundsMap)
			{
				object->UseTexConfig(true, textureType);
			}
		}
	}
	void SetTextureSpawn(TextureType textureType)
	{
		for (const auto& [name, actors] : graphManager->GetAllComponents())
		{
			for (const auto& [name, object] : actors->uSpawnerMap)
			{
				object->UseTexture_all(textureType);
			}
		}
	}
	void RotateBound(const std::string& sceneComponentName, const std::string& boundName,  glm::vec3 orientation)
	{
		if (!graphManager->HasComponent(sceneComponentName))
		{
			std::cerr << "[ERROR]: SceneGraph component not found for rotation handling.\n";
			return;
		}
		auto sceneGraphComponent = graphManager->GetComponent(sceneComponentName);

		if (!sceneGraphComponent)
		{
			std::cerr << "[ERROR]: Invalid SceneGraph component retrieved.\n";
			return;
		}

		const auto& bounds = sceneGraphComponent->uBoundsMap.find(boundName)->second;
		bounds->SetLocalRotation(orientation);
	}

	//Collision
	void AABBInverseActor(const std::string& sceneComponentName, const std::string& actorName, const std::string& boundsName)
	{
		if (!graphManager->HasComponent(sceneComponentName))
		{
			std::cerr << "[ERROR]: SceneGraph component not found for collision handling.\n";
			return;
		}
		auto sceneGraphComponent = graphManager->GetComponent(sceneComponentName); 

		if (!sceneGraphComponent)
		{
			std::cerr << "[ERROR]: Invalid SceneGraph component retrieved.\n";
			return;
		}

		const auto& actor = sceneGraphComponent->uActorMap.find(actorName)->second;
		const auto& bounds = sceneGraphComponent->uBoundsMap.find(boundsName)->second;
		actor->EnablePhysics(true);

		if (!actor || !bounds) 
		{
			std::cerr << "[ERROR]: Actor or bounds object is null.\n";
			return;
		}

		if (Collision::AABBInverse(actor, bounds)) 
		{
			actor->rigidB->velocity.y = 0.f;
			 actor->rigidB->Roll(glm::vec3(0.0f,-1.0f,0.0f));
		}
	}
	void AABBInverseSpawner(const std::string& sceneComponentName, const std::string& spawnerName, const std::string& boundsName)
	{
		if (!graphManager->HasComponent(sceneComponentName))
		{
			std::cerr << "[ERROR]: SceneGraph component not found for collision handling.\n";
			return;
		}
		auto sceneGraphComponent = graphManager->GetComponent(sceneComponentName);

		if (!sceneGraphComponent)
		{
			std::cerr << "[ERROR]: Invalid SceneGraph component retrieved.\n";
			return;
		}

		const auto& spawner = sceneGraphComponent->uSpawnerMap.find(spawnerName)->second;
		const auto& bounds = sceneGraphComponent->uBoundsMap.find(boundsName)->second;
		for (size_t  i = 0; i < spawner->spawnVector.size(); i++)
		{
			if (!spawner || !bounds)
			{
				std::cerr << "[ERROR]: Actor or bounds object is null.\n";
				return;
			}
			const auto& particle = spawner->spawnVector[i];
			particle->EnablePhysics(true);
			bool isColliding = Collision::AABBInverse(particle, bounds);

			if (isColliding) 
			{
				particle->rigidB->velocity.y = 0.f;
				//Check for collison with other actors
				for (size_t j = i + 1; j < spawner->spawnVector.size(); j++) 
				{
					const auto& otherParticle = spawner->spawnVector[j]; 

					Collision::BallBall<Actor, Actor>(particle, otherParticle); 
					//particle->rigidB->velocity.y = 0.f;

				}
			}
		}
	}

	void SetPositionSpawn(const std::string& sceneComponentName, const std::string& spawnerName, const glm::vec3& newPos) const 
	{
		if (!graphManager->HasComponent(sceneComponentName))
		{
			std::cerr << "[ERROR]: SceneGraph component not found for collision handling.\n";
			return;
		}
		auto sceneGraphComponent = graphManager->GetComponent(sceneComponentName);

		if (!sceneGraphComponent)
		{
			std::cerr << "[ERROR]: Invalid SceneGraph component retrieved.\n";
			return;
		}

		const auto& spawner = sceneGraphComponent->uSpawnerMap.find(spawnerName)->second;
		for (size_t  i = 0; i < spawner->spawnVector.size(); i++) 
		{
			if (spawner->spawnVector[i]->mEnablePhysics == true)
			{
				std::cout << "[LOG]:RigidB Pos Updated\n";
				spawner->spawnVector[i]->rigidB->SetLocalPosition(newPos);

			}
			std::cout << "[LOG]:Normal Pos Updated\n";
			spawner->spawnVector[i]->SetLocalPosition(newPos);
		}
	}
	void SetPositionActor(const std::string& sceneComponentName, const std::string& actorName, const glm::vec3& newPos) const
	{
		if (!graphManager->HasComponent(sceneComponentName))
		{
			std::cerr << "[ERROR]: SceneGraph component not found for collision handling.\n";
			return;
		}
		auto sceneGraphComponent = graphManager->GetComponent(sceneComponentName);

		if (!sceneGraphComponent)
		{
			std::cerr << "[ERROR]: Invalid SceneGraph component retrieved.\n";
			return;
		}
		auto& actor = sceneGraphComponent->uActorMap.find(actorName)->second;
		
		if (!actor->mEnablePhysics)
		{
			std::cout << "[LOG]:Normal Pos Updated\n";
			actor->SetLocalPosition(newPos);
		}
		std::cout << "[LOG]:RigidB Pos Updated\n";
		actor->rigidB->SetLocalPosition(newPos);
		return; 
	}

	glm::vec3 GetPositionBounds(const std::string& sceneComponentName, const std::string& boundsName)
	{
		if (!graphManager->HasComponent(sceneComponentName))
		{
			std::cerr << "[ERROR]: SceneGraph component not found for collision handling.\n";
			return glm::vec3(0.f);
		}
		auto sceneGraphComponent = graphManager->GetComponent(sceneComponentName);

		if (!sceneGraphComponent)
		{
			std::cerr << "[ERROR]: Invalid SceneGraph component retrieved.\n";
			return glm::vec3(0.f);
		}

		const auto& bound = sceneGraphComponent->uBoundsMap.find(boundsName)->second->GetLocalPosition();
		return bound; 
	}
};

