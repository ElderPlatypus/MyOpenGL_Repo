#include "RigidBody.h"

RigidBody::RigidBody(float mass, const glm::vec3& pos, const glm::vec3& velocity, const glm::vec3& acceleration)
	:mass(mass),pos(pos), velocity(velocity), acceleration(acceleration) {}

void RigidBody::Update(float dt)
{
	pos += velocity * dt + 0.5f * acceleration * glm::pow(dt, 2.0f);
	velocity += acceleration * dt;
}
