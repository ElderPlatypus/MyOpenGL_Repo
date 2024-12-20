#include "RigidBody.h"

RigidBody::RigidBody(float mass, const glm::vec3& pos, const glm::vec3& velocity, const glm::vec3& acceleration)
	:mass(mass),pos(pos), velocity(velocity), acceleration(acceleration) {}

void RigidBody::Update(float dt, const glm::vec3& minBound, const glm::vec3& maxBound)
{
	velocity += acceleration * dt - friction;
	pos += velocity * dt + 0.5f * acceleration * (dt * dt); 

	ConstraintPosition(minBound, maxBound);
	velocity *= 0.98f;
}

void RigidBody::Update(float dt)
{
	velocity += acceleration * dt - friction; 
	pos += velocity * dt + 0.5f * acceleration * (dt*dt); 

	//velocity *= 0.99f;  
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

void RigidBody::ApplyFriction(float newFriction)
{
	//std::cout << "[LOG]:Friction Applied \n";
	friction = newFriction;
}

void RigidBody::EnergyTransfer(float joules)
{
	if (joules == 0) return;


	float deltaV = glm::sqrt(2 * glm::abs(joules) / mass);

	velocity += joules > 0 ? deltaV : -deltaV;
}

void RigidBody::RigidBodyStop()
{
	velocity = glm::vec3(0.0f);
	acceleration = glm::vec3(0.0f);
}

void RigidBody::Reflect(const glm::vec3& normal, float restitution)
{
	velocity = velocity - (1.0f + restitution) * glm::dot(velocity, normal) * normal;
}

void RigidBody::ConstraintPosition(const glm::vec3& minBound, const glm::vec3& maxBound)
{
	for (int i = 0; i < 3; i++)
	{
		if (pos[i] < minBound[i])
		{
			pos[i] = minBound[i];
			velocity[i] = 0.0f;
		}
		if (pos[i] > maxBound[i]) 
		{
			pos[i] = maxBound[i];
			velocity[i] = 0.0f;
		}
	}
}

void RigidBody::Roll(const glm::vec3& surfaceCoords)
{
	// Normalize the surface normal
	glm::vec3 normalizedSurface = glm::normalize(surfaceCoords);

	// Calculate rolling acceleration: projection of gravity onto the surface
	acceleration = gravity - glm::dot(gravity, normalizedSurface) * normalizedSurface;
}

void RigidBody::SetLocalPosition(const glm::vec3& _pos)
{
	pos = _pos;
}

glm::vec3 RigidBody::GetLocalPosition() const
{
	return pos;
}

void RigidBody::SetMass(float _mass)
{
	mass = _mass;
}

