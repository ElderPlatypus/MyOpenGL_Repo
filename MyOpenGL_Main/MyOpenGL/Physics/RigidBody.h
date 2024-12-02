#pragma once

//External Includes
#include <glm/glm.hpp>

//Full Inclusion
#include "../MathLib/Formulas.h"

class RigidBody
{
public:
	float mass;

	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	RigidBody(float mass = 1.0f, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& velocity = glm::vec3(0.0f), const glm::vec3& acceleration = glm::vec3(0.0f));
	void Update(float dt, const glm::vec3& minBound, const glm::vec3& maxBound);
	void Update(float dt);

	//Apply Forces
	void ApplyForce(glm::vec3 force);
	void ApplyForce(glm::vec3 direction, float magnitude);
	void ApplyImpulse(glm::vec3 force, float dt);
	void ApplyImpulse(glm::vec3 direction, float magnitude,  float dt);

	//Energy Tranfers
	void EnergyTransfer(float joules);

	//Full stop
	void RigidBodyStop();

	//Reflect
	void Reflect(const glm::vec3& normal, float restitution = 0.5f);

	//Domain constraint
	void ConstraintPosition(const glm::vec3& minBound, const glm::vec3& maxBound);

	void Roll(const glm::vec3& baryCoords);

	void SetLocalPosition(const glm::vec3& _pos);
	glm::vec3 GetLocalPosition() const;
};

