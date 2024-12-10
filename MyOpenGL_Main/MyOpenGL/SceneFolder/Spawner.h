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

class Spawner
{
	//Find actor helper function
	auto FindActor(const std::shared_ptr<Actor>& actor)
	{
		return std::find(spawnVector.begin(), spawnVector.end(), actor);
	}

	float elapsedTime = 0.0f;
public:
	std::vector<std::shared_ptr<Actor>> spawnVector{};
	std::vector<std::shared_ptr<Actor>> projectileList{};

	Spawner() = default;
	Spawner(int spawnAmount, MeshShape shape);
	Spawner(int spawnAmount, float scale,  MeshShape shape);
	Spawner(int spawnAmount, float distriButionX, float distriButionZ, MeshShape shape);
	Spawner(int spawnAmount, float distriButionX, float distriButionZ, float scale, MeshShape shape);
	Spawner(const std::shared_ptr<Actor>& actor, float dt);

	~Spawner();

	//Render
	void SetShaderAll(const std::shared_ptr<Shader>& shader);
	void UpdateAll(float dt);

	//Deletion
	void ClearSpawnvector_single(const std::shared_ptr<Actor>& actor);
	void ClearSpawnvector_all();

	//Light
	void UseTexture_all(TextureType textureType);
	void UseTexture_single(const std::shared_ptr<Actor>& actor, TextureType textureType);

	//Light
	void UseLight_all(LightType lightType);
	void UseLight_single(const std::shared_ptr<Actor>& actor, LightType lightType);

	//Barycentric Coordinates
	void SetBarySurfaceMesh_all(const std::shared_ptr<Mesh>& _selectSurface);
	void UpdateBaryCentriCoordinates_all(float dt);
	void SetBarySurfaceMesh_single(const std::shared_ptr<Actor>& actor, const std::shared_ptr<Mesh>& _selectSurface);
	void UpdateBaryCentriCoordinates_single(const std::shared_ptr<Actor>& actor, float dt);

	//Enable states
	void EnablePhysics_all(const bool& enablePhysics);
	void EnablePhysics_single(const std::shared_ptr<Actor>& actor, const bool& enablePhysics);

	//Position
	void SetPosition_all(const glm::vec3& position);

	//LifeTime
	float maxLifeTime = 10.0f;
	bool IsExpired() const; 

private:
	float mScale = 1.0f;

	std::shared_ptr<Actor> CreateActor(MeshShape shape, int index, float scale);
	std::shared_ptr<Actor> CreateActor(MeshShape shape, int index);

	
};

