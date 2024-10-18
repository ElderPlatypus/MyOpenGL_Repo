#pragma once

//External
#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <memory>

//Includes
#include "../CameraFolder/Camera.h"
#include "../Shaders/Shader.h"
#include "../Utility/Transform.h"
#include "../SceneFolder/Actor.h"
#include "../SceneFolder/Mesh.h"
#include "../Application.h"
#include "../CameraFolder/Camera.h"
#include "../Shaders/Texture.h"
#include "../Physics/Collision.h"
#include "../ECS/Entity.h"
#include "../ECS/Systems.h"
#include "../ECS/Components.h"

class Scene 
{
	std::string mName;

public:
	std::unordered_map<std::string,std::shared_ptr<Actor>> uActorMap;
	std::shared_ptr<Shader> mShader;
	std::shared_ptr<Camera> mSceneCamera; 
	std::shared_ptr<Texture> mTexture;
	Scene(std::string name);
	Scene() = default;


	//Loaders
	void LoadActors();
	void LoadContent(); 
	void UnloadContent();

	//Updaters
	void UpdateScene(float dt);

	//Renderer
	void RenderScene(float dt, Transform globalTransform = {});
	Transform mTransform;

	//Actor space manipulation
	void SpaceManipulation();

	//Camera Binding
	void BindCamera() const;

	//Collision control
	Collision* mCollision{ nullptr };
	void CollisionHandling(float dt); 

	int score = 0;

	//ECS
	int numEntities = 0;
	std::vector<std::shared_ptr<Entity>> mEntities;

	std::shared_ptr<MovementSystem> movementSystem; 
	std::shared_ptr<CollisionSystem> collisionSystem;
	std::shared_ptr<ActorSystem> actorSystem;
	std::shared_ptr<HealthSystem> healthSystem; 


	std::shared_ptr<ComponentArchive<TransformComponent>> transformManager = std::make_shared<ComponentArchive<TransformComponent>>();
	std::shared_ptr<ComponentArchive<HealthComponent>> healthManager = std::make_shared<ComponentArchive<HealthComponent>>();
	std::shared_ptr<ComponentArchive<BoxCollisionComponent>> AABBManager = std::make_shared<ComponentArchive<BoxCollisionComponent>>();
	std::shared_ptr<ComponentArchive<ActorComponent>> actorManager = std::make_shared<ComponentArchive<ActorComponent>>();


};

