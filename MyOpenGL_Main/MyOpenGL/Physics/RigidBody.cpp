#include "RigidBody.h"

RigidBody::RigidBody(float mass, const glm::vec3& pos, const glm::vec3& velocity, const glm::vec3& acceleration)
	:mass(mass),pos(pos), velocity(velocity), acceleration(acceleration) {}

void RigidBody::Update(float dt, const glm::vec3& minBound, const glm::vec3& maxBound)
{
	pos += velocity * dt + 0.5f * acceleration * glm::pow(dt, 2.0f);
	velocity += acceleration * dt;

	ConstraintPosition(minBound, maxBound);
	velocity *= 0.98f;
}

void RigidBody::Update(float dt)
{
	pos += velocity * dt + 0.5f * acceleration * glm::pow(dt, 2.0f);
	velocity += acceleration * dt;

	//velocity *= 0.98f; 
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

void RigidBody::Roll(const glm::vec3& baryCoords)
{
	 acceleration =  acceleration + glm::vec3(baryCoords.x*baryCoords.z,baryCoords.y*baryCoords.z,glm::pow(baryCoords.z,2)-1);
}

void RigidBody::SetLocalPosition(const glm::vec3& _pos)
{
	pos = _pos;
}

glm::vec3 RigidBody::GetLocalPosition() const
{
	return pos;
}

