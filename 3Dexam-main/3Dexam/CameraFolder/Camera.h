#pragma once

//Includes
#include <string>
#include <map>
#include "../Utility/Transform.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Direction
{
	forward, backwards,
	right, left,
	up, down,
	speed
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
		float farPlane = 100.f,
		float FOV = 45.f,
		float maxMovementSpeed = 40.f,
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
	//______________Vector handling_____________
	glm::vec3 GetForwardVector() const;
	glm::vec3 GetUpVector() const;
	//______________Matrix handling_____________
	glm::mat4 GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const;
	glm::mat4 GetFrustumMatrix() const;
	//______________Speed handling_____________
	const glm::vec3& GetVelocity() const { return mVelocity; }
	const glm::vec3& GetAcceleration() const { return mAcceleration; }
	float GetAccelerationSpeed() const { return mAccelerationSpeed; }
	float GetMaxMovementSpeed() const { return mMaxMovementSpeed; }
	//______________View_____________
	float GetAspectRatio() const { return mAspectRatio; }
	

    ///Transformation
   //---------------------------------Local Setters------------------------------------------------
	void SetTransformation(const Transform& transform) { mTransform = transform; }
	void SetLocalPosition(const glm::vec3& position) { mTransform.SetPosition(position); }
	void SetLocalRotation(const glm::quat& rotation) { mTransform.SetRotation(rotation); }
	void SetLocalScale(const glm::vec3& scale) { mTransform.SetScale(scale); }
	void SetLocalTransformMatrix(const glm::mat4& transformMatrix) { mTransform.SetTransformMatrix(transformMatrix); }


	//---------------------------------Loacal Getters------------------------------------------
	const Transform& GetTransform() const { return mTransform; }
	const glm::vec3& GetLocalPosition() const { return mTransform.GetPosition(); }
	const glm::quat& GetLocalRotation() const { return mTransform.GetOrientation(); }
	const glm::vec3& GetLocalScale() const { return mTransform.GetScale(); }
	const glm::mat4 GetLocalTransformMatrix() const { return mTransform.GetTransformMatrix(); }
	glm::vec3 GetRightVector() const { return mTransform.GetRightVector(); }
	float GetYaw() { return mTransform.GetYaw(); }
	float GetPitch()  { return mTransform.GetPitch(); }

	///Movement members
	glm::vec3 mVelocity{ 0.f,0.f,0.f };
	glm::vec3 mAcceleration{ 0.f,0.f,0.f };
	float mDampeningFactor;
	float mMaxMovementSpeed;
	float mAccelerationSpeed;

	float mAspectRatio;
	float mNearPlane;
	float mFarplane;
	float mFOV;

	///Mouse members
	std::string mName;
	glm::mat4 mProjectionMatrix;
	Transform mTransform{};

	void CameraMovement(Direction direction ,float dt);
	

};

