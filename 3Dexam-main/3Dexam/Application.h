#pragma once
//Includes std
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>

//Other includes
#include "glm/mat4x3.hpp"
#include "SceneFolder/Scene.h"

class Application
{
public:
	struct GLFWwindow* mWindow{ nullptr };

	///Create and Destroy window object
	Application() = default;  
	Application(std::string name, int width, int height) : mWidth(width), mHeight(height), mName(name) {}
	~Application(); //Destroy window memory loaction after done running 

	static Application* GetAPP();

	///Initializers
	void GLFW_Init();
	void Window_Init(); //Initialize window

	///RenderLoop
	void Run_App(); 
	void Original_Run_App();

	///Window Callbacks
	void RegisterWindowCallbacks();
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);

	///Setters
	void SetHeight(int height);
	void SetWidth(int width);

	///Window KeyController
	std::map<int, bool> mKeyState; 
	std::unordered_map<std::string, class Scene* > uSceneMap;

	void ExitApplication(float dt);

	int mWidth{ 1920 };
	int mHeight{ 1080 };

	///Callbacks for camera
	void UpdateActiveController(float dt, std::string);

	std::string mName;
};