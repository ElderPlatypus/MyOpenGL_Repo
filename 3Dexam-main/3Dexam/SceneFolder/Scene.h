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

class Scene 
{
	std::string mName;
	std::unordered_map<std::string,Actor*> uActorMap;


public:
	Shader* mShader{ nullptr };
	Camera* mSceneCamera{ nullptr };
	Texture* mTexture{ nullptr }; 
	Scene(std::string name);

	std::vector<Actor*> spawnVector;

	//Loaders
	void LoadActors();
	void LoadContent(); //Load all content in scene
	void UnloadContent();
	void Spawner();

	//Updaters
	void UpdateScene(float dt);

	//Renderer
	void RenderScene(float dt, Transform globalTransform = {});
	Transform mTransform;

	//Actor space manipulation
	void SpaceManipulation();

	//Camera Binding
	void BindCamera();

	Scene() = default;

	
	
};

