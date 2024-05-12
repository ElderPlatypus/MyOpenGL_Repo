#pragma once

//Includes
#include <string>
#include <map>
#include "../Utility/Transform.h"
#include "../SceneFolder/Actor.h"
#include "../Utility/InputInterface.h"

class Camera : public Actor
{
public:
	Camera(
		const std::string& name,
		Actor* actor,
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
		glm::quat rotation = glm::quat(glm::identity<glm::quat>()),
		float aspectRatio = 1280.f / 720.f, 
		float nearPlane = 0.1f, 
		float farPlane = 100.f, 
		float FOV = 45.f,
		float maxMovementSpeed = 50.f, 
		float accelerationSpeed = 60.f 
		 );

	//Setters & Adders
	void AddVelocity(const glm::vec3& velocity) { mVelocity += velocity; }
	void SetAspectRatio(float aspectRatio);
	void SetAccelerationSpeed(float accelerationSpeed) { mAccelerationSpeed = accelerationSpeed; }
	void SetAcceleration(const glm::vec3 acceleration) { mAcceleration = mAcceleration; }
	void SetMaxMovementSpeed(float maxMovementSpeed) { mMaxMovementSpeed = maxMovementSpeed; }
	void SetVelocity(const glm::vec3 velocity) { mVelocity = velocity; }

	//Updaters
	void UpdateCamera(float dt);
	//______________Updates all speed function_____________
	//void UpdateSpeed(float dt);
	void UpdateVelocity(float dt);
	void UpdatePosition(float dt);
	//______________Updates all martixes_____________
	void UpdateProjectionMatrix();
	
	//Getters
	//______________Vector handling_____________
	glm::vec3 GetForwardVector() const;
	glm::vec3 GetUpVector() const;
	//______________Matrix handling_____________
	glm::mat4 GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const;
	glm::mat4 GetFrustumMatrix() const;
	//______________Speed handling_____________
	glm::vec3& GetVelocity() { return mVelocity; }
	glm::vec3& GetAcceleration() { return mAcceleration; }
	float GetAccelerationSpeed() { return mAccelerationSpeed; }
	float GetMaxMovementSpeed() { return mMaxMovementSpeed; }
	//______________View_____________
	float GetAspectRatio() { return mAspectRatio; }
	

	//CamerController


	//Movement members
	glm::vec3 mVelocity{ 0.f,0.f,0.f };
	glm::vec3 mAcceleration{ 0.f,0.f,0.f };
	float mMaxMovementSpeed;
	float mAccelerationSpeed;

	float mAspectRatio;
	float mNearPlane;
	float mFarplane;
	float mFOV;
	
	glm::mat4 mProjectionMatrix;

	Actor* mActor{ nullptr };
	

};

