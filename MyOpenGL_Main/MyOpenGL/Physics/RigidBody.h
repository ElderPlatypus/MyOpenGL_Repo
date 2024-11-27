#pragma once

//External Includes
#include <glm/glm.hpp>

class RigidBody
{
public:
	float mass;

	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	RigidBody(float mass = 1.0f, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& velocity = glm::vec3(0.0f), const glm::vec3& acceleration = glm::vec3(0.0f));
	void Update(float dt);
	void ApplyForce(glm::vec3 force);
	void ApplyForce(glm::vec3 direction, float magnitude);
	void ApplyImpulse(glm::vec3 force, float dt);
	void ApplyImpulse(glm::vec3 direction, float magnitude,  float dt);
	void energyTransfer(float joules);

};

