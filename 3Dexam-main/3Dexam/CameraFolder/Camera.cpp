#include "Camera.h"
#include <iostream>
#include "../Application.h"


Camera::Camera(const std::string& name,
	glm::vec3 position,
	glm::quat rotation,
	float aspectRatio,
	float nearPlane,
	float farPlane,
	float FOV,
	float maxMovementSpeed,
	float accelerationSpeed)
{
	    mName = name;
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
	CameraMovement(forward,dt);  
	CameraMovement(backwards, dt);
	CameraMovement(left, dt);
	CameraMovement(right, dt); 

}

void Camera::UpdateVelocity(float dt)
{
	//Updating the velocity 
	mVelocity.z = 100.f; 

	//If the velocity exceeds the maximum speed, it is normalized: (unity vector) with
	//size of 1 and the multiplied with the max speed until criteria is not true.
	if (glm::length(mVelocity) > mMaxMovementSpeed) 
	{
		mVelocity = glm::normalize(mVelocity) * mMaxMovementSpeed; 

		/*std::cout << "Check velocity vec: " << mVelocity.x << " "
			<< mVelocity.y << " "
			<< mVelocity.z << " "
			<< std::endl;*/
	}
	
	mAcceleration = glm::vec3(0.f);
}

void Camera::UpdatePosition(float dt)
{
	
	glm::vec3 front = GetForwardVector();
	glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));//Creating a normalized vector of the crossproduct of frontVec and upVec as 1 since its normalized. (a new orthogonal vector, or rigthVec in this case).
	glm::vec3 up = glm::cross(right, front);  
	 
	//Updating the position as a 
	SetLocalPosition(GetLocalPosition() + mVelocity.x * dt * right + mVelocity.y * dt * up + mVelocity.z * dt * front);
	//std::cout << "Check acceleration pos: " <<
	//	GetLocalPosition().x << " "
	//	<< GetLocalPosition().y << " "
	//	<< GetLocalPosition().z << " "
	//	<< std::endl;

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

///Camera movement
void Camera::CameraMovement(Direction direction, float dt)
{

	//Forward and Backwards
	if (direction == forward) mAcceleration.z += mAccelerationSpeed;  
	if (direction == backwards) mAcceleration.z -= GetAccelerationSpeed();

	//Left and right
	if (direction == right) mAcceleration.x += GetAccelerationSpeed(); 
	if (direction == left) mAcceleration.x -= GetAccelerationSpeed(); 

	//Up and Down
	if (direction == up) mAcceleration.y += GetAccelerationSpeed();
	if (direction == down) mAcceleration.y -= GetAccelerationSpeed(); 

	//Increase of Speed
	if (direction == speed) SetAccelerationSpeed(200.f);
	else if (direction != speed) SetAccelerationSpeed(50.f);

	SetAcceleration(mAcceleration);   

	//std::cout << "Actor pos:" << scene-> << " " << getCameraAcceleration.y << " " << getCameraAcceleration.z << std::endl;
	//std::cout << "Check acceleration pos: " << mAcceleration.x << " " 
	//	<< mAcceleration.y << " "
	//	<< mAcceleration.z << " " 
	//	<< std::endl;
}



