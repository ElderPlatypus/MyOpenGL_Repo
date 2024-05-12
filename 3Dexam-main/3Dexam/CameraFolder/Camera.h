#pragma once

//Includes
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>


class Camera
{
public:
	Camera(
		const std::string& name,
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
		glm::vec3 target = glm::vec3(0.f, 0.f, 0.f),
		glm::vec3 reverseDirection = glm::normalize(glm::vec3(0.f, 0.f, 0.f)- glm::vec3(0.f, 0.f, 0.f)),
		float aspectRatio = 1280.f / 720.f, 
		float nearPlane = 0.f, 
		float farPlane = 500.f, 
		float FOV = 45.f,
		float maxMovementSpeed = 50.f, 
		float accelerationSpeed = 60.f 
		 );

	void ProjectionMatrix();
	void UpdateCamera(float dt);


private:
	//Updatable variables
	glm::vec3 mPosition;
	glm::vec3 mReverseDirection = glm::normalize(mPosition - mTarget);
	glm::vec3 mTarget;
	glm::mat4 mProjectionMatrix;

	float mAspectRatio;
	float mNearPlane;
	float mfarplane;
	float mFOV;
	float mMaxMovementSpeed;
	float mAccelerationSpeed;

	//Essential variables pre-set
	glm::vec3 mUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 mXaxis = glm::normalize(glm::cross(mUp, mReverseDirection)); //presents the x-axis
	glm::vec3 mYaxis = glm::normalize(glm::cross(mReverseDirection, mXaxis)); //presents the y-axis

	glm::mat4 view = glm::lookAt
	(   glm::vec3(0.f, 0.f, 3.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f)
	);
	
};

