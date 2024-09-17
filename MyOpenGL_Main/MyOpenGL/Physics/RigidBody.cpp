#include "RigidBody.h"

RigidBody::RigidBody(float mass, const glm::vec3& pos, const glm::vec3& velocity, const glm::vec3& acceleration)
	:mass(mass),pos(pos), velocity(velocity), acceleration(acceleration) {}

void RigidBody::Update(float dt)
{
	pos += velocity * dt + 0.5f * acceleration * glm::pow(dt, 2.0f);
	velocity += acceleration * dt;
}

void RigidBody::ApplyForce(glm::vec3 force)
{
	acceleration += force / mass;
}

void RigidBody::ApplyForce(glm::vec3 direction, float magnitude)
{
	ApplyForce(direction * magnitude); 
}

void RigidBody::ApplyImpulse(glm::vec3 force, float dt)
{
	velocity += force / mass * dt; 
}

void RigidBody::ApplyImpulse(glm::vec3 direction, float magnitude, float dt)
{
	ApplyImpulse(direction * magnitude,dt);
}

void RigidBody::energyTransfer(float joules)
{
	if (joules == 0) return;


	float deltaV = glm::sqrt(2 * glm::abs(joules) / mass);

	velocity += joules > 0 ? deltaV : -deltaV;

}


