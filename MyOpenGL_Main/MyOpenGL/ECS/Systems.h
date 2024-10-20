#pragma once

//External
#include <typeindex>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <stdexcept>

//classes
#include "Components.h"
#include "Entity.h"
#include "../SceneFolder/Actor.h"

///_________________________System Interface Class___________________________
struct ISystem
{
	virtual void Update(const std::vector<std::shared_ptr<Entity>>& _entities)
	{
		std::cout << "[LOG]:ISystem Updating \n";
	}
};

class MovementSystem : public ISystem
{
private:
	std::shared_ptr<ComponentArchive<TransformComponent>> m_transformManager; 

public:
	MovementSystem(const std::shared_ptr<ComponentArchive<TransformComponent>>& _transformManager) 
	{
		this->m_transformManager = _transformManager;
	}

	void Update(const std::vector<std::shared_ptr<Entity>>& _entities) override
	{
		std::cout << "[LOG]:MovementSystem Updating \n";
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_transformManager->HasComponent(entity->GetId()))
			{
				const std::shared_ptr<TransformComponent>& position = m_transformManager->GetComponent(entity->GetId());
				m_transformManager->AddComponent(entity->GetId(), position);
			}
			else
			{
				throw std::runtime_error("[WARNING]:Entity Id or TransformComponent not found \n");
			}
		}
		std::cout << "\n\n";
	}
};

class HealthSystem : public ISystem
{
private:
	std::shared_ptr<ComponentArchive<HealthComponent>> m_healthManager; 

public:
	HealthSystem(const std::shared_ptr<ComponentArchive<HealthComponent>>& _m_healthManager) 
	{
		this->m_healthManager = _m_healthManager; 
	}

	void Update(const std::vector<std::shared_ptr<Entity>>& _entities) override
	{
		std::cout << "[LOG]:MovementSystem Updating \n";
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_healthManager->HasComponent(entity->GetId()))
			{
				std::shared_ptr<HealthComponent> position = m_healthManager->GetComponent(entity->GetId());
				m_healthManager->AddComponent(entity->GetId(), position);
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
	}
};

class CollisionSystem : public ISystem
{
private:
	std::shared_ptr<ComponentArchive<BoxCollisionComponent>> m_AABB_Manager;

public:
	CollisionSystem(const std::shared_ptr<ComponentArchive<BoxCollisionComponent>>& _m_AABB_Manager)
	{
		this->m_AABB_Manager = _m_AABB_Manager;
	}

	void Update(const std::vector<std::shared_ptr<Entity>>& _entities) override
	{
		std::cout << "[LOG]:CollisionSystem Updating \n";
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_AABB_Manager->HasComponent(entity->GetId()))
			{
				std::shared_ptr<BoxCollisionComponent> collision = m_AABB_Manager->GetComponent(entity->GetId()); 
				m_AABB_Manager->AddComponent(entity->GetId(), collision); 
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
	}
};

class ActorSystem : public ISystem
{
private:
	std::shared_ptr<ComponentArchive<ActorComponent>> m_actorManager;

public:
	ActorSystem(const std::shared_ptr<ComponentArchive<ActorComponent>>& _m_actorManager)
	{
		this->m_actorManager = _m_actorManager;
	}
    
	void Update(const std::vector<std::shared_ptr<Entity>>& _entities) override
	{
		std::cout << "[LOG]:ActorSystem Updating \n";
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_actorManager->HasComponent(entity->GetId()))
			{
		
				const std::shared_ptr<ActorComponent>& actor = m_actorManager->GetComponent(entity->GetId());
				for (const auto& actor : actor->actors)
				{
					//m_actorManager->GetComponent(entity->GetId())->actors[entity->GetId()]->isPlayer = true;
					/*actor->isPlayer = true;
					actor->isActor = true;*/
				}
				m_actorManager->AddComponent(entity->GetId(), actor); 
				
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
	}

	void SetShader(const std::vector<std::shared_ptr<Entity>>& _entities, const std::shared_ptr<Shader>& shader) const 
	{
		std::cout << "[LOG]:ActorSystem SetShader \n";
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_actorManager->HasComponent(entity->GetId()))
			{
				const std::shared_ptr<ActorComponent>& actor = m_actorManager->GetComponent(entity->GetId());
				for (const std::shared_ptr<Actor>& it : actor->actors)
				{
					it->SetShader(shader);
				}
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
	}

	void DrawActor(const std::vector<std::shared_ptr<Entity>>& _entities, const std::shared_ptr<Shader>& shader) const
	{
		std::cout << "[LOG]:ActorSystem SetShader \n";
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_actorManager->HasComponent(entity->GetId()))
			{
				const auto& actor = m_actorManager->GetComponent(entity->GetId());
				for (const std::shared_ptr<Actor>& it : actor->actors)
				{
					it->GetdrawActor(shader); 
				}
				//m_actorManager.AddComponent(entity->GetId(), actor);
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
		std::cout << "\n\n";
	}

	void UpdateActorEntity(const std::vector<std::shared_ptr<Entity>>& _entities, float dt) const
	{
		for (const auto& entity : _entities)
		{
			if (m_actorManager->HasComponent(entity->GetId()))
			{
				const std::shared_ptr<ActorComponent>& actor = m_actorManager->GetComponent(entity->GetId()); 
				for (const auto& it : actor->actors)
				{
					it->UpdateActors(dt);
				}
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or TransformComponent not found \n";
			}
		}
	}

	std::shared_ptr<Actor> GetActor(const std::vector<std::shared_ptr<Entity>>& _entities)
	{
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_actorManager->HasComponent(entity->GetId()))
			{
				  return m_actorManager->GetComponent(entity->GetId())->actors[entity->GetId()];
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
				return nullptr;
			}
		}
		return nullptr;
	}

	bool AttachToCamera(const std::vector<std::shared_ptr<Entity>>& _entities, const int& id)
	{
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_actorManager->HasComponent(id)) 
			{
				return m_actorManager->GetComponent(entity->GetId())->actors[entity->GetId()]->mAttachToActor = true;
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
				return false;
			}
		}
		return false;
	}
};