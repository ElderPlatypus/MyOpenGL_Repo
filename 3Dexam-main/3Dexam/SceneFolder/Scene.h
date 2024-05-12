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

class Scene 
{
	std::string mName;
	std::unordered_map<std::string,Actor*> uActorMap;


public:
	Shader* mShader{ nullptr };
	Camera* mSceneCamera{ nullptr };
	Scene(std::string name);

	//Loaders
	void LoadActors();
	void LoadContent(); //Load all content in scene
	void UnloadContent();

	//Updaters
	void UpdateScene(float dt);

	//Renderer
	void RenderScene(float dt, Transform globalTransform = Transform{});

	//Actor space manipulation
	void SpaceManipulation();

	//Camera Binding
	void BindCamera();

	//Callbacks
	//void FramebufferSizeCallback(class Application* app, int width, int height);
	//void KeyCallback(class Application* app, int key, int scancode, int action, int mods);
	//void MouseButtonCallback(class Application* app, int button, int action, int mods);
	//void CursorPosCallback(class Application* app, double xPos, double yPos);

	
	//std::shared_ptr<Camera> mCameraController; 

	Scene() = default;
	
};

