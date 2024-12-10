#pragma once
//Icludes c++
#include <vector>
#include <memory>
#include <random>
#include <unordered_map>
#include <string>
#include <iostream>
#include <glm/glm.hpp> 

//Full inclusion
#include "../SceneFolder/Actor.h"
#include "../Utility/EnumArchive.h"
#include "../SceneFolder/Spawner.h"
class Shader;

class ParticleSimulation
{
	ParticleType pt{};
	MeshShape shape;
	std::vector<std::shared_ptr<Spawner>> spawners; 

	float elapsedTime = 0.0f;
	float spawnInterval = 2.0f;

	int particleNumber = 0;
	float origin = 0.f;
	float area = 0.f; 
	int index = 0;
	std::shared_ptr<Actor> bound = nullptr;
	std::unordered_map<std::string, std::shared_ptr<Actor>> objects = { {} };


public:
	ParticleSimulation(int _particleNumber = 500, MeshShape _shape = MeshShape::Sphere, ParticleType _pTypes = ParticleType::None);
	~ParticleSimulation();

	void Update(float dt);
	void SetShader(const std::shared_ptr<Shader>& shader);

	void SnowParticle(const float& _origin, const float& _area, const std::shared_ptr<Actor>& _bound, const std::unordered_map<std::string, std::shared_ptr<Actor>>& _object);
	void UpdateSnowPartices(float dt); 

	void RainParticle(const float& _origin, const float& _area, const std::shared_ptr<Actor>& _bound, const std::unordered_map<std::string, std::shared_ptr<Actor>>& _object);
	void UpdateRainParticles(float dt);
private:
	void SetPosition(const glm::vec3& pos);
	glm::vec3 GetPosition();
	std::shared_ptr<Spawner> CreateNewSpawn();
	void UpdateSystem(float dt);
};

