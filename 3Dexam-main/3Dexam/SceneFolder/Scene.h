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


class Scene : public Application
{
	std::string mName;
	std::unordered_map<std::string,Actor*> uActorMap;


public:
	Shader* mShader{ nullptr };
	Scene(std::string name);

	//Loaders
	void LoadActors();
	void LoadContent(); //Load all content in scene
	void UnloadContent();

	void RenderScene(float dt, Transform globalTransform = Transform{});

	void SpaceManipulation();
	
};

