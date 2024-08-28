#pragma once

//Includes
#include <glad/glad.h>
#include <iostream> 

#include "../Definitions.h"
#include "../Shaders/Shader.h"
#include "../SceneFolder/Actor.h"

class Light
{
public:
	Light(Shader* shader);
	Light(Shader* shader, Actor* actor);
	~Light() = default;
	Light(const Light&) = delete;

	float mAmbientStrength = 1.0f;
	glm::vec3 mLightColor{ 1.f, 1.f, 1.f };
	glm::vec3 mLightPos{ 0.f,0.f,0.f }; 
	glm::vec3 mObjectColor{ 1.f,0.7f,0.2f }; 

	void setAmbientStrength(float newAmbientStrength);
	void setLightColor(glm::vec3 newLightColor);
	void setObjectColor(glm::vec3 newObjectColor);

	Shader* mShader{nullptr};
	Actor* mActor{ nullptr };
};

