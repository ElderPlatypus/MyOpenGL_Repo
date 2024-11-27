#pragma once

//Includes c++
#include <iostream>
#include <string>

//External Includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Full incluson
#include "../MathLib/Transform.h"
#include "../Utility/EnumArchive.h"


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
	void UpdatePosition(float dt) const;
	//______________Updates all martixes_____________
	void UpdateProjectionMatrix();
	
	///Getters
	
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
    //---------------------------------Methods Setters------------------------------------------------
	void SetTransformation(const std::shared_ptr<Transform>& transform) { mTransform = transform; }
	void SetLocalPosition(const glm::vec3& position) const { mTransform->SetPosition(position); }
	void SetLocalRotation(const glm::quat& rotation) const { mTransform->SetRotation(rotation); }
	void SetLocalScale(const glm::vec3& scale) const { mTransform->SetScale(scale); }
	void SetLocalTransformMatrix(const glm::mat4& transformMatrix) const { mTransform->SetTransformMatrix(transformMatrix); }
	//---------------------------------Methods Getters------------------------------------------
	std::shared_ptr<Transform> GetTransform() const { return mTransform; }
	const glm::vec3& GetLocalPosition() const { return mTransform->GetPosition(); }
	const glm::quat& GetLocalRotation() const { return mTransform->GetOrientation(); }
	const glm::vec3& GetLocalScale() const { return mTransform->GetScale(); }

	glm::mat4 GetLocalTransformMatrix() const { return mTransform->GetTransformMatrix(); }
	const glm::vec3 GetForwardVector() const { return mTransform->GetForwardVector(); }
	const glm::vec3 GetRightVector() const { return mTransform->GetRightVector(); }
	const glm::vec3 GetUpVector() const 
	{ 
		//Return Up vector
		glm::vec3 front = GetForwardVector();
		glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));
		return glm::rotate(GetLocalRotation(), glm::vec3(0.f, 1.f, 0.f));
	}
	float GetPitch() const { return mTransform->GetPitch(); }
	float GetYaw() const { return mTransform->GetYaw(); }

	///Members
	//---------------------------------Members------------------------------------------
	std::shared_ptr<Transform> mTransform = std::make_shared<Transform>();
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
	void CameraMouseMovement(double xPos, double yPos) const;
};