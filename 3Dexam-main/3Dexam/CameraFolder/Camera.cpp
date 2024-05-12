#include "Camera.h"


Camera::Camera(
	const std::string& name,
	glm::vec3 position,
	glm::vec3 target,
	glm::vec3 reverseDirection,
	float aspectRatio,
	float nearPlane,
	float farPlane,
	float FOV,
	float maxMovementSpeed,
	float accelerationSpeed
)

{
	mPosition = position;
	mTarget = target;
	mReverseDirection = reverseDirection;

	mAspectRatio = aspectRatio;
	mNearPlane = nearPlane;
	mfarplane = farPlane;
	mFOV = FOV;
	mMaxMovementSpeed = maxMovementSpeed;
	mAccelerationSpeed = accelerationSpeed;
}

void Camera::ProjectionMatrix()
{
	mProjectionMatrix = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearPlane, mfarplane);
}

void Camera::UpdateCamera(float dt)
{
	ProjectionMatrix();
}
