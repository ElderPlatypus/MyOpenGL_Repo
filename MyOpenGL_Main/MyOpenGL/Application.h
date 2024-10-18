#pragma once

//External
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/matrix_transform.hpp"

//Classes
#include "SceneFolder/Scene.h"
#include "Shaders/ShaderFileLoader.h"
#include "Shaders/Shader.h"


class Application
{
public:

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

	///Window Callbacks
	void RegisterWindowCallbacks();
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height) const;
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);

	///Setters
	void SetHeight(int height);
	void SetWidth(int width);

	///Members and variables
	std::map<int, bool> mKeyState; 
	std::map<int, bool> mButtonState;
	struct GLFWwindow* mWindow{ nullptr };
	class Scene* mScene{ nullptr };
	int mWidth{ 1920 };
	int mHeight{ 1080 };
	std::string mName;

	///Util Funtions
	void ExitApplication(float dt);

	//---------------------------------Update callback from Camera class------------------------------------------
	void UpdateCameraController(float dt);
	void UpdateActorMovement(float dt);
	void UpdateCameraPlacement(float dt);


};