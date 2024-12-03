#pragma once

//Icludes c++
#include <vector>
#include <memory>
#include <random>
#include <string>
#include <iostream>
#include <glm/glm.hpp> 

//Full inclusion
#include "../SceneFolder/Actor.h"
class Shader;

class Spawner : public Actor 
{
	//Find actor helper function
	static auto FindActor(const std::shared_ptr<Actor>& actor)
	{
		return std::find(spawnList.begin(), spawnList.end(), actor);
	}
public:
	inline static std::vector<std::shared_ptr<Actor>> spawnList;
	inline static std::vector<std::shared_ptr<Actor>> projectileList;

	static std::vector<std::shared_ptr<Actor>> SpawnActors(int spawnAmount, float distriButionX, float distriButionZ, MeshShape shape)
	{
		if (spawnAmount <= 0)
		{
			std::cerr << "[WARING]:Spawn Amount must be greater than 0 \n";
			return {};
		}

		//Init vector and reserve memory
		//std::vector<std::shared_ptr<Actor>> actors;
		spawnList.reserve(spawnAmount);

		// Random data setup
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<float> distrX(-distriButionX, distriButionX);
		std::uniform_real_distribution<float> distrZ(-distriButionZ, distriButionZ);

		//Spawn the vectors
		for (int i = 0; i < spawnAmount; i++)
		{
			std::shared_ptr<Actor> actor = CreateActor(shape, i); 
			if (!actor) continue;

			glm::vec3 randomPosition(distrX(eng), 0.0f, distrZ(eng));
			actor->SetLocalPosition(randomPosition);
			actor->rigidB->pos = randomPosition;
			spawnList.emplace_back(actor);
		}
		return spawnList;
	}
	static std::vector<std::shared_ptr<Actor>> SpawnActors(int spawnAmount, float distriButionX, float distriButionZ,float scale, MeshShape shape)
	{
		if (spawnAmount <= 0)
		{
			std::cerr << "[WARING]:Spawn Amount must be greater than 0 \n";
			return {};
		}

		//Init vector and reserve memory
		std::vector<std::shared_ptr<Actor>> actors;
		actors.reserve(spawnAmount);

		// Random data setup
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<float> distrX(-distriButionX, distriButionX);
		std::uniform_real_distribution<float> distrZ(-distriButionZ, distriButionZ);

		//Spawn the vectors
		for (int i = 0; i < spawnAmount; i++)
		{
			std::shared_ptr<Actor> actor = CreateActor(shape, i, scale); 
			if (!actor) continue;

			glm::vec3 randomPosition(distrX(eng), 0.0f, distrZ(eng));
			actor->SetLocalPosition(randomPosition);
			actor->rigidB->pos = randomPosition;
			actors.emplace_back(actor);
		}
		return actors;
	}
	static std::vector<std::shared_ptr<Actor>> ProjectileSpawner(const std::shared_ptr<Actor>& actor, float dt)
	{
		float timer = dt;
		int index = 0;
		if (isShooting)
		{
			index++;
			projectileActor = CreateActor(MeshShape::Sphere,index);
			projectileActor->SetLocalPosition(actor->GetLocalPosition().z + actor->mExtent*0.5f + (10.f * dt));
			projectileActor->SetLocalRotation(actor->GetLocalRotation());
			projectileList.emplace_back(projectileActor); 
		}
		isShooting = false;
	}

	//Render
	static void SetShaderSpawnVector(const std::shared_ptr<Shader>& shader)  
	{
		for (const auto& content : spawnList)
		{
			content->SetShader(shader);
		}
	}
	inline static void UpdateSpawnVector(float dt)
	{
		for (const auto& actor : spawnList) 
		{
			actor->UpdateActors(dt);
		}
	}

	//Deletion
	static void DeleteSpawnvector_single(const std::shared_ptr<Actor>& actor)
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if(spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}
		
		//using std::find to locate the specified actor in the list
	    auto it = FindActor(actor); 

		//Iterating over list of actors in list
	    for (auto& actor : spawnList)
	    {
			//If the actor is not the last element in the list it gets deleted
			if (it != spawnList.end())
			{
				spawnList.erase(it);
				std::cout << "[LOG]:Actor Deleted Successfully \n";
			}
			else
			{
				std::cerr << "[WARNING]:Actor not found in the list \n";
			}
	    }
		std::cout << "\n";
	}
	static void DeleteSpawnvector_all()
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty()) 
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		//Iterating over list of actors in list
		for (const auto& actor : spawnList)
		{
			actor->~Actor();
		}
		spawnList.clear(); 
		std::cout << "[LOG]:Actors Deleted Successfully \n";
		std::cout << "\n";
	} 

	//Light
	static void UseTexture_all(TextureType textureType)
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		for (const auto& actor : spawnList)
		{
			actor->UseTexConfig(true, textureType);
			//std::cout << "[LOG]:Actor Texture Updated Successfully \n";
		}
		//std::cout << "\n";
	}
	static void UseTexture_single(const std::shared_ptr<Actor>& actor, TextureType textureType)
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		//using std::find to locate the specified actor in the list
		auto it = FindActor(actor);

		//Iterating over list of actors in list
		for (auto& actor : spawnList)
		{
			//If the actor is not the last element in the list it gets deleted
			if (it != spawnList.end())
			{
				it->get()->UseTexConfig(true, textureType); 
				std::cout << "[LOG]:Actor Texture Updated Successfully \n";
			}
			else
			{
				std::cerr << "[WARNING]:Actor not found in the list \n";
			}
		}
	}

	//Light
	static void UseLight_all(LightType lightType)
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		for (const auto& actor : spawnList)
		{
			actor->UseLightConfig(true, lightType);
			//std::cout << "[LOG]:Actor Lighting Updated Successfully \n";
		}
		//std::cout << "\n";
	}
	static void UseLight_single(const std::shared_ptr<Actor>& actor, LightType lightType)
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		//using std::find to locate the specified actor in the list
		auto it = FindActor(actor); 

		//Iterating over list of actors in list
		for (auto& actor : spawnList)
		{
			//If the actor is not the last element in the list it gets deleted
			if (it != spawnList.end())
			{
				it->get()->UseLightConfig(true,lightType); 
				//std::cout << "[LOG]:Actor Lighting Updated Successfully \n";
			}
			else
			{
				std::cerr << "[WARNING]:Actor not found in the list \n";
			}
		}
		//std::cout << "\n";
	}

	//Barycentric Coordinates
	static void SetBarySurfaceMesh_all(const std::shared_ptr<Mesh>& _selectSurface)
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		for (const auto& actor : spawnList)
		{
			actor->SetBarySurfaceMesh(_selectSurface);
			//std::cout << "[LOG]:Actor BarySurface Updated Successfully \n";
		}
		//std::cout << "\n";
	}
	static void UpdateBaryCentriCoordinates_all(float dt)
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		for (const auto& actor : spawnList)
		{
			MathLib::DoBarycentricCoordinatesMesh<Mesh,Mesh>(actor->getMesh(), actor->mSurfaceMesh, dt);
			//std::cout << "[LOG]:Actor BarySurface Updated Successfully \n";
		}
		//std::cout << "\n";
	}
	static void SetBarySurfaceMesh_single(const std::shared_ptr<Actor>& actor, const std::shared_ptr<Mesh>& _selectSurface) 
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		//using std::find to locate the specified actor in the list
		auto it = FindActor(actor);

		//Iterating over list of actors in list
		for (auto& actor : spawnList)
		{
			//If the actor is not the last element in the list it gets deleted
			if (it != spawnList.end())
			{
				it->get()->SetBarySurfaceMesh(_selectSurface); 
				//std::cout << "[LOG]:Actor BarySurface Updated Successfully \n";
			}
			else
			{
				std::cerr << "[WARNING]:Actor not found in the list \n";
			}
		}
	}
	static void UpdateBaryCentriCoordinates_single(const std::shared_ptr<Actor>& actor, float dt)
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		//using std::find to locate the specified actor in the list
		auto it = FindActor(actor);

		//Iterating over list of actors in list
		for (auto& actor : spawnList)
		{
			//If the actor is not the last element in the list it gets deleted
			if (it != spawnList.end())
			{
				it->get()->UpdateBarycentricCoords(dt);
				//std::cout << "[LOG]:Actor BarySurface Updated Successfully \n";
			}
			else
			{
				std::cerr << "[WARNING]:Actor not found in the list \n";
			}
		}
	}
	static void ClearBarySurfaceMesh_all() 
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		for (const auto& actor : spawnList)
		{
			actor->mSurfaceMesh = nullptr;
			//std::cout << "[LOG]:Actor BarySurface Unloaded Successfully \n";
		}
		//std::cout << "\n";
	}

	//Enable states
	static void EnablePhysics_all(const bool& enablePhysics)
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		for (const auto& actor : spawnList)
		{
			actor->EnablePhysics(enablePhysics); 
			//std::cout << "[LOG]:Actor Physics Updated Successfully \n";
		}
		//std::cout << "\n";
	}
	static void EnablePhysics_single(const std::shared_ptr<Actor>& actor, const bool& enablePhysics)
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		//using std::find to locate the specified actor in the list
		auto it = FindActor(actor);

		//Iterating over list of actors in list
		for (auto& actor : spawnList)
		{
			//If the actor is not the last element in the list it gets deleted
			if (it != spawnList.end())
			{
				it->get()->EnablePhysics(enablePhysics);
				//std::cout << "[LOG]:Actor Physics Updated Successfully \n";
			}
			else
			{
				std::cerr << "[WARNING]:Actor not found in the list \n";
			}
		}
		//std::cout << "\n";
	}

	//Position
	static void SetPosition_all(const glm::vec3& position)  
	{
		//Fail test to see if the method contains any actors to delete, if yes progress
		if (spawnList.empty())
		{
			std::cerr << "[WARNING]:Spawn contains zero elements/is empty \n";
			return;
		}

		for (auto& actor : spawnList)
		{
			if (!actor)
			{
				std::cerr << "[WARNING]:Invalid Actor \n";
			}
			else if(actor->rigidB)
			{
				actor->rigidB->pos = actor->rigidB->pos + position;
			}
			else
			{
				actor->SetLocalPosition(position + actor->GetLocalPosition());
			}
			//std::cout << "[LOG]:Actor Position Updated Successfully \n";
		}
		//std::cout << "\n";
	}

private:
	inline static float mScale = 1.0f;

	static std::shared_ptr<Actor> CreateActor(MeshShape shape, int index, float scale)
	{
		mScale = scale;
		switch (shape)
		{
		case Cube:
			return std::make_shared<Actor>(Mesh::CreateCube(mScale), "Cube_ " + std::to_string(index));
		case Sphere:
			return std::make_shared<Actor>(Mesh::CreateSphere(16, 16, static_cast<int>(mScale)), "Sphere_ " + std::to_string(index));
		case Pyramid:
			return std::make_shared<Actor>(Mesh::CreatePyramid(mScale), "Pyramid_ " + std::to_string(index));
		default:
			std::cerr << "[WARNING]:No shape selected or uknown type \n";
			return nullptr;
		}
	}
	static std::shared_ptr<Actor> CreateActor(MeshShape shape, int index)
	{
		switch (shape)
		{
		case Cube:
			return std::make_shared<Actor>(Mesh::CreateCube(mScale), "Cube_" + std::to_string(index));
		case Sphere:
			return std::make_shared<Actor>(Mesh::CreateSphere(16, 16, static_cast<int>(mScale)), "Sphere_" + std::to_string(index));
		case Pyramid:
			return std::make_shared<Actor>(Mesh::CreatePyramid(mScale), "Pyramid_" + std::to_string(index));
		default:
			std::cerr << "[WARNING]:No shape selected or uknown type \n";
			return nullptr;
		}
	}

	
};

