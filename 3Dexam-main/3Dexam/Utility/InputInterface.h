#pragma once

class IController
{
public:
	virtual ~IController() = default;

	virtual void UpdateActiveController(float dt) = 0;
	virtual void FramebufferSizeCallback(class Application* app, int width, int height) = 0;
	virtual void KeyCallback(class Application* app, int key, int scancode, int action, int mods) = 0;
	virtual void MouseButtonCallback(class Application* app, int button, int action, int mods) = 0;
	virtual void CursorPosCallback(class Application* app, double xPos, double yPos) = 0;
};
