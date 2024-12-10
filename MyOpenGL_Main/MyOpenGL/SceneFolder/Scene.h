#pragma once

//Includes c++
#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <memory>
#include <string>

//Full Inclusion
#include "../Camera/Camera.h"
#include "../Shaders/Shader.h"
#include "../Shaders/ShaderFileLoader.h"
#include "../MathLib/Transform.h"
#include "../SceneFolder/Actor.h"
#include "../SceneFolder/Spawner.h"
#include "../SceneFolder/Mesh.h"
#include "../Application.h"
#include "../Shaders/Texture.h"
#include "../Physics/Collision.h"
#include "../ECS/Systems.h"
#include "../ECS/Components.h"
#include "../MathLib/Formulas.h"
#include "../Utility/FileHandler.h"
#include "../Physics/ParticleSimulation.h" 

class Scene 
{
	std::string mName;

public:
	std::unordered_map<std::string,std::shared_ptr<Actor>> uActorMap;
	std::unordered_map<std::string, std::shared_ptr<Texture>> uTextureMap; 
	std::unordered_map<std::string, std::shared_ptr<ParticleSimulation>> uParticleMap;
	std::unordered_map<std::string, std::shared_ptr<Spawner>> uSpawnerMap;
	std::unordered_map<std::string, std::shared_ptr<Actor>> uBoundingMap;


	std::vector<std::unordered_map<std::string, std::shared_ptr<Actor>>> uMaps;

	std::shared_ptr<Shader> mShader;
	std::shared_ptr<Camera> mSceneCamera; 

	Scene(std::string name);
	Scene() = default;

	//Loaders
	void LoadActors();
	void LoadContent(); 
	void UnloadContent();

	//Updaters
	void UpdateScene(float dt);
	void Update_uMapActors(float dt);
	void Set_uMapShader(const std::shared_ptr<Shader>& shader);

	//Renderer
	void RenderScene(float dt, Transform globalTransform = {});
	Transform mTransform;

	//Actor space manipulation
	void SpaceManipulation() const;

	//Camera Binding
	void BindCamera() const;

	//Collision control
	Collision* mCollision{ nullptr };
	void CollisionHandling(float dt); 


	//ECS
	int numEntities = 0;
	int score = 0;

	//Components
	std::shared_ptr<ComponentArchive<SceneGraphComponent>> sceneGraphManager = std::make_shared<ComponentArchive<SceneGraphComponent>>();
	std::shared_ptr<ComponentArchive<TransformComponent>> transformManager = std::make_shared<ComponentArchive<TransformComponent>>();
	std::shared_ptr<ComponentArchive<PhysicsComponent>> physicsManager = std::make_shared<ComponentArchive<PhysicsComponent>>();
	std::shared_ptr<ComponentArchive<HealthComponent>> healthManager = std::make_shared<ComponentArchive<HealthComponent>>();
	std::shared_ptr<ComponentArchive<DamageComponent>> damageManager = std::make_shared<ComponentArchive<DamageComponent>>();


	std::vector<std::vector<std::string>> terrainData;

};

