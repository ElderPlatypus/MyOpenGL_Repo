#pragma once

//Includes standard lib
#include <iostream>
#include <vector>
#include <unordered_map>

//Includes
#include "../CameraFolder/Camera.h"
#include "../Shaders/Shader.h"
#include "../Utility/Transform.h"
#include "../SceneFolder/Actor.h"
#include "../Application.h"
#include "../CameraFolder/Camera.h"
#include "../Shaders/Texture.h"
#include "../Utility/Collision.h"

class Scene 
{
	std::string mName;


public:
	std::unordered_map<std::string,Actor*> uActorMap;
	Shader* mShader{ nullptr };
	Camera* mSceneCamera{ nullptr };
	Texture* mTexture{ nullptr }; 
	Scene(std::string name);


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

	Scene() = default;

	//Spawner controll
	int mAmount = 10;

	//Collision control
	Collision* mCollision{ nullptr };
};

