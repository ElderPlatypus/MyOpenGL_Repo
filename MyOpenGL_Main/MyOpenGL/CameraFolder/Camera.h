#pragma once

//External
#include <iostream>
#include <string>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Classes
#include "../Utility/Transform.h"

//Enum
enum Direction
{
	//Directions
	Forward, Backwards,
	Right, Left,
	Up, Down,
	IncreaseSpeed,

	//Mouse buttons
	RightMouseButton,
	MouseMotionX, MouseMotionY,

	//Other buttons
	CameraFreeMovment_1,
	CameraStatic_CharacterMovement_2,
	CameraStatic_FollowPlayer_3
};


class Camera
{
public:
	Camera(
		const std::string& name,
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
		glm::quat rotation = glm::quat(glm::identity<glm::quat>()),
		float aspectRatio = 1280.f / 720.f,
		float nearPlane = 0.1f,
		float farPlane = 200.f,
		float FOV = 45.f,
		float maxMovementSpeed = 100.f,
		float accelerationSpeed = 50.f,
		float dampeningFactor = 5.f
		 );

	///Setters & Adders
	void AddVelocity(const glm::vec3& velocity) { mVelocity += velocity; }
	void SetAspectRatio(float aspectRatio);
	void SetAccelerationSpeed(float accelerationSpeed) { mAccelerationSpeed = accelerationSpeed; }
	void SetAcceleration(const glm::vec3& acceleration) { mAcceleration = acceleration; }
	void SetMaxMovementSpeed(float maxMovementSpeed) { mMaxMovementSpeed = maxMovementSpeed; }
	void SetVelocity(const glm::vec3& velocity) { mVelocity = velocity; }

	///Updaters
	void UpdateCamera(float dt) ;
	//______________Updates all speed function_____________
	void UpdateVelocity(float dt);
	void UpdateDampening(float dt);
	void UpdatePosition(float dt);
	//______________Updates all martixes_____________
	void UpdateProjectionMatrix();
	
	///Getters
	//---------------------------------Vector Methods Getters--------------------------------------------
	glm::vec3 GetForwardVector() const;
	glm::vec3 GetUpVector() const;
	//---------------------------------Matrix Methods Getters--------------------------------------------
	glm::mat4 GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const;
	glm::mat4 GetFrustumMatrix() const;
	//---------------------------------Speed Methods Getters--------------------------------------------
	const glm::vec3& GetVelocity() const { return mVelocity; }
	const glm::vec3& GetAcceleration() const { return mAcceleration; }
	float GetAccelerationSpeed() const { return mAccelerationSpeed; }
	float GetMaxMovementSpeed() const { return mMaxMovementSpeed; }
	//---------------------------------AspectARtio Methods Getter--------------------------------------------
	float GetAspectRatio() const { return mAspectRatio; }
	

   ///Transformation
   //---------------------------------Methods Setters--------------------------------------------
	void SetTransformation(const Transform& transform) { mTransform = transform; }
	void SetLocalPosition(const glm::vec3& position) { mTransform.SetPosition(position); }
	void SetLocalRotation(const glm::quat& rotation) { mTransform.SetRotation(rotation); }
	void SetLocalScale(const glm::vec3& scale) { mTransform.SetScale(scale); }
	void SetLocalTransformMatrix(const glm::mat4& transformMatrix) { mTransform.SetTransformMatrix(transformMatrix); }
	//---------------------------------Methods Getters------------------------------------------
	const Transform& GetTransform() const { return mTransform; }
	const glm::vec3& GetLocalPosition() const { return mTransform.GetPosition(); }
	const glm::quat& GetLocalRotation() const { return mTransform.GetOrientation(); }
	const glm::vec3& GetLocalScale() const { return mTransform.GetScale(); }
	const glm::mat4 GetLocalTransformMatrix() const { return mTransform.GetTransformMatrix(); }
	glm::vec3 GetRightVector() const { return mTransform.GetRightVector(); }

	///Members
	//---------------------------------Members------------------------------------------
	Transform mTransform{};
	std::string mName;
	glm::vec3 mVelocity{ 0.f,0.f,0.f };
	glm::vec3 mAcceleration{ 0.f,0.f,0.f };
	glm::mat4 mProjectionMatrix;
	float mDampeningFactor;
	float mMaxMovementSpeed; 
	float mAccelerationSpeed;
	float mAspectRatio;
	float mNearPlane;
	float mFarplane;
	float mFOV;

	///Mouse 
	//---------------------------------Members------------------------------------------
	float mLastX; 
	float mLastY; 
	bool mRightMouseButtonPressed = false;
	float mMouseSensitivity = 0.05f;

	///CamerControll
	//---------------------------------Members------------------------------------------
	bool mUseCameraMovement = true; 
	//---------------------------------Methods------------------------------------------
	void CameraMovement(Direction direction,float dt);
	void CameraMouseButton(double xPos, double yPos);
	void CameraMouseMovement(double xPos, double yPos);
};

