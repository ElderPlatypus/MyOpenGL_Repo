#pragma once
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

};

