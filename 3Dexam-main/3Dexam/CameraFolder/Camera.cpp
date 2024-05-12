#include "Camera.h"

Camera::Camera(const std::string& name,
	Actor* actor,
	glm::vec3 position,
	glm::quat rotation,
	float aspectRatio,
	float nearPlane,
	float farPlane,
	float FOV, 
	float maxMovementSpeed,
	float accelerationSpeed) : mActor(actor) 
{
	
		mMaxMovementSpeed = maxMovementSpeed;
		mAccelerationSpeed = accelerationSpeed;

		mAspectRatio = aspectRatio;
		mNearPlane = nearPlane;
		mFarplane = farPlane;
		mFOV = FOV;
		
		SetLocalPosition(position); 
		SetLocalRotation(rotation); 
		UpdateProjectionMatrix();  
}

void Camera::SetAspectRatio(float aspectRatio)
{
	mAspectRatio = aspectRatio;
	UpdateProjectionMatrix(); 
}


void Camera::UpdateCamera(float dt)
{
	
	UpdateVelocity(dt);
	UpdatePosition(dt);
}

void Camera::UpdateVelocity(float dt)
{
	//Updating the velocity 
	mVelocity += mAcceleration * dt;

	//If the velocity exceeds the maximum speed, it is normalized: (unity vector) with
	//size of 1 and the multiplied with the max speed until criteria is not true.
	if (mVelocity.length() > mMaxMovementSpeed)
	{
		mVelocity = glm::normalize(mVelocity) * mMaxMovementSpeed;
	}
	mAcceleration = glm::vec3(0.f);
}

void Camera::UpdatePosition(float dt)
{
	glm::vec3 front = GetForwardVector();
	glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));//Creating a normalized vector of the crossproduct of frontVec and upVec as 1 since its normalized. (a new orthogonal vector, or rigthVec in this case).
	glm::vec3 up = glm::cross(front, right); 

	//Updating the position as a 
	SetLocalPosition(GetLocalPosition() + (mVelocity.x * dt * right) + (mVelocity.y * dt * up) + (mVelocity.z * dt * front));
}

void Camera::UpdateProjectionMatrix()
{
	mProjectionMatrix = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearPlane, mFarplane);
}

glm::vec3 Camera::GetForwardVector() const
{
	//Returns Forward vector
	return glm::rotate(GetLocalRotation(),glm::vec3(0.f,0.f,-1.f));
}

glm::vec3 Camera::GetUpVector() const
{
	//Return Up vector
	return glm::rotate(GetLocalRotation(), glm::vec3(0.f, 1.f, 0.f));
}

glm::mat4 Camera::GetViewMatrix() const
{
	//Returns the viewMatrix
	return glm::lookAt(GetLocalPosition(), GetLocalPosition() + GetForwardVector(), glm::vec3(0.f, 1.f, 0.f)); 
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

glm::mat4 Camera::GetFrustumMatrix() const
{
	return mProjectionMatrix * GetViewMatrix(); 
}




