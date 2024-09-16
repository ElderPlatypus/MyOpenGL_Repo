#include "Camera.h"

Camera::Camera(const std::string& name,
	glm::vec3 position,
	glm::quat rotation,
	float aspectRatio,
	float nearPlane,
	float farPlane,
	float FOV,
	float maxMovementSpeed,
	float accelerationSpeed,
    float dampeningFactor)
{
	    mName = name;
		mMaxMovementSpeed = maxMovementSpeed;
		mAccelerationSpeed = accelerationSpeed;
		mDampeningFactor = dampeningFactor;
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
	UpdateDampening(dt);
}

void Camera::UpdateVelocity(float dt)
{
	//Updating the velocity 
	mVelocity += mAcceleration * dt; 

	//If the velocity exceeds the maximum speed, it is normalized: (unity vector) with
	//size of 1 and the multiplied with the max speed until criteria is not true.
	if (glm::length(mVelocity) > mMaxMovementSpeed) 
	{
		mVelocity = glm::normalize(mVelocity) * mMaxMovementSpeed; 
	}
	
 	mAcceleration = glm::vec3(0.f);
}

void Camera::UpdateDampening(float dt)
{
	float dampingFactor = glm::length(mVelocity) > 0 ? mDampeningFactor : 0.0f;

	mVelocity -= mVelocity * dampingFactor * dt;

	if (glm::length(mVelocity) < 0.001f)
	{
		mVelocity = glm::vec3(0.0f);
	}
}

void Camera::UpdatePosition(float dt)
{
	
	glm::vec3 front = GetForwardVector();
	glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));//Creating a normalized vector of the crossproduct of frontVec and upVec as 1 since its normalized. (a new orthogonal vector, or rigthVec in this case).
	glm::vec3 up = glm::cross(right, front);  
	 
	//Updating the position as a 
	SetLocalPosition(GetLocalPosition() + mVelocity.x * dt * right + mVelocity.y * dt * up + mVelocity.z * dt * front);
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
	glm::vec3 front = GetForwardVector();
	glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));
	return glm::rotate(GetLocalRotation(), glm::vec3(0.f, 1.f, 0.f));
}

glm::mat4 Camera::GetViewMatrix() const
{
	//Returns the viewMatrix
	return glm::lookAt(GetLocalPosition(), GetLocalPosition() + GetForwardVector(), GetUpVector()); 
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
	if (!mUseCameraMovement) return;

    switch (direction)
    {
    	//Forward and Backwards
        case Forward: mAcceleration.z += GetAccelerationSpeed(); break;
        case Backwards:mAcceleration.z -= GetAccelerationSpeed(); break;;
        case Right:mAcceleration.x += GetAccelerationSpeed(); break;
        case Left:mAcceleration.x -= GetAccelerationSpeed(); break;
        case Up:mAcceleration.y += GetAccelerationSpeed(); break;
        case Down:mAcceleration.y -= GetAccelerationSpeed(); break;
        case IncreaseSpeed: mAcceleration *= 3.0f, mMaxMovementSpeed = 70.f; break;
        default: mAcceleration = glm::vec3(0.f, 0.f, 0.f), mMaxMovementSpeed = 50.f;
    }
	
}

void Camera::CameraMouseButton(double xPos, double yPos)
{
	//Saves the x and y pos for lather when rotating camera
	if (mUseCameraMovement)
	{
		mRightMouseButtonPressed = true;
		mLastX = static_cast<float>(xPos);
		mLastY = static_cast<float>(yPos);
	}
}

void Camera::CameraMouseMovement(double xPos, double yPos) 
{
	if (!mRightMouseButtonPressed) return;

	else
	{

		/*std::cout << "Mouse Button Pressed \n" << std::endl;*/

		float xOffset = mLastX - static_cast<float>(xPos);
		float yOffset = mLastY - static_cast<float>(yPos);

		xOffset *= mMouseSensitivity;
		yOffset *= mMouseSensitivity;

		float yawRadians = glm::radians(xOffset);
		float pitchRadians = glm::radians(yOffset);


		//Creating a quaternion from angle and a normalized axis: projecting the normalized axis with the given angel whic is now the rotation domain for the quaternion.
		//Can bed visualize as a  cone where the flat surface is the where the quaternions merges from.
		glm::quat currentOrientation = GetLocalRotation();
		glm::quat yawRotation = glm::angleAxis(yawRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //normal axis is not 0 for y-axis --> gets yaw
		glm::quat pitchRotation = glm::angleAxis(pitchRadians, glm::vec3(1.0f, 0.0f, 0.0f));  //normal axis is not 0 for x-axis --> gets pitch
		glm::quat newOrientation = yawRotation * currentOrientation * pitchRotation;
		newOrientation = glm::normalize(newOrientation);
		SetLocalRotation(newOrientation);
	}
	
}











