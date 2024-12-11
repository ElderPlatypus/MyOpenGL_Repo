#pragma once

//Full inclusion
#include "../Physics/Environment.h"

//External includes
#include <random>

struct Collision
{
	template<typename Actor, typename Actor1>
	static bool AABB(const std::shared_ptr<Actor>& a, const std::shared_ptr<Actor1>& b)
	{
		//Flag if an actor is nullpointer
		if (!a || !b) return false;
		
		//Calculate the difference between center
		glm::vec3 centerDiff = glm::abs(a->GetCenter() - b->GetCenter()); 

		//Calculate the sum of extents
		glm::vec3 sumOfExtent = a->GetExtent() + b->GetExtent();  
	
		//Checking if one of the axis is not overlaping
		for (int i = 0; i < 3; i++)
		{
			if (centerDiff[i] >= sumOfExtent[i]) 
			{
				return false;
			}
		}
		return true;
	}

	template<typename Actor, typename BoundingRegionActor>
	static bool AABBInverse(const std::shared_ptr<Actor>& a, const std::shared_ptr<BoundingRegionActor>& b) 
	{
		// Ensure both actors are valid before proceeding
		if (!a || !b) return false;

		// Calculate the difference between the centers of the two bounding regions
		glm::vec3 centerDiff = a->GetCenter() - b->GetCenter();

		// Retrieve the extents of both bounding boxes (half-sizes for each axis)
		glm::vec3 aExtent = a->GetExtent(); // Half-size of bounding box for Actor a
		glm::vec3 bExtent = b->GetExtent(); // Half-size of bounding box for Actor b

		// Calculate the overlap on each axis
		// Overlap is determined as the absolute center difference minus the combined extents
		glm::vec3 overlap = glm::abs(centerDiff) - (bExtent - aExtent);

		// Flag to indicate if Actor a is outside the bounds of Actor b
		bool outOfBound = false;

		// Generate random velocity perturbation for realism 
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<float> random(-0.5f, 0.5f);
		glm::vec3 randVec(random(eng), random(eng), random(eng));

		// Check for overlap on the X-axis
		if (overlap.x > 0)
		{
			// Reverse velocity on the X-axis and add a random perturbation
			a->rigidB->velocity.x *= -1;
			a->rigidB->velocity += randVec;

			// Reposition Actor a within the legal bounds of Actor b on the X-axis
			a->rigidB->SetLocalPosition(glm::vec3(
				b->GetCenter().x + glm::sign(centerDiff.x) * (bExtent.x - aExtent.x),
				a->rigidB->GetLocalPosition().y,
				a->rigidB->GetLocalPosition().z));

			outOfBound = true;
		}

		// Check for overlap on the Y-axis
		if (overlap.y > 0)
		{
			// Reverse velocity on the X-axis and add a random perturbation
			a->rigidB->velocity.y *= -1;
			a->rigidB->velocity += randVec;

			// Reposition Actor a within the legal bounds of Actor b on the Y-axis
			a->rigidB->SetLocalPosition(glm::vec3(
				a->rigidB->GetLocalPosition().x,
				b->GetCenter().y + glm::sign(centerDiff.y) * (bExtent.y - aExtent.y),
				a->rigidB->GetLocalPosition().z));

			outOfBound = true;
		}

		// Check for overlap on the Z-axis
		if (overlap.z > 0)
		{
			// Reverse velocity on the Z-axis and add a random perturbation
			a->rigidB->velocity.z *= -1;
			a->rigidB->velocity += randVec;

			// Reposition Actor a within the legal bounds of Actor b on the Z-axis
			a->rigidB->SetLocalPosition(glm::vec3(
				a->rigidB->GetLocalPosition().x,
				a->rigidB->GetLocalPosition().y,
				b->GetCenter().z + glm::sign(centerDiff.z) * (bExtent.z - aExtent.z)));

			outOfBound = true;
		}

		return outOfBound; // Return whether Actor a is out of bounds and required repositioning
	}

	template<typename Actor, typename Actor1>
	static bool BallBall(const std::shared_ptr<Actor>& a, const std::shared_ptr<Actor1>& b)
	{
		// Ensure that both actors are valid (not null)
		if (!a || !b) return false;

		// Calculate the vector difference between the centers of both actors
		glm::vec3 centerDiff = a->GetCenter() - b->GetCenter();
		float distance = glm::length(centerDiff); // Calculate the distance between the centers

		// Compute the sum of the radius (extent.x) of both actors (assuming spherical or roughly spherical bounding boxes)
		float combinedRadius = a->GetExtent().x + b->GetExtent().x;

		// Check if the distance between the centers is less than the combined radius
		// If true, the actors are overlapping and a collision has occurred
		if (distance < combinedRadius)
		{
			// Normalize the center difference to get the direction of the collision normal
			glm::vec3 collisionNormal = glm::normalize(centerDiff);

			// Calculate the penetration depth, which is the amount by which the actors are overlapping
			float penetrationDepth = combinedRadius - distance;

			// Adjust the positions of both actors to resolve the overlap
			// Each actor is moved by half of the penetration depth along the collision normal
			a->rigidB->SetLocalPosition(a->rigidB->GetLocalPosition() + collisionNormal * (penetrationDepth * 0.5f));
			b->rigidB->SetLocalPosition(b->rigidB->GetLocalPosition() - collisionNormal * (penetrationDepth * 0.5f));

			// Compute the relative velocity between the two actors along the collision normal
			glm::vec3 relativeVelocity = a->rigidB->velocity - b->rigidB->velocity;
			float impactSpeed = glm::dot(relativeVelocity, collisionNormal); // The speed at which they are closing along the normal

			// If the impact speed is negative, it means the actors are moving towards each other
			// We calculate the impulse required to reflect them off each other
			if (impactSpeed < 0)
			{
				// Impulse is proportional to the impact speed and the collision normal
				glm::vec3 impulse = collisionNormal * impactSpeed;

				// Apply the impulse to both actors' velocities to simulate the collision response
				a->rigidB->velocity -= impulse; // Actor 'a' slows down
				b->rigidB->velocity += impulse; // Actor 'b' speeds up
			}

			// Return true to indicate that a collision has occurred
			return true;
		}

		// Return false if no collision is detected
		return false;
	}

	template<typename Actor, typename Actor1>
	static bool BallBallInverse(const std::shared_ptr<Actor>& a, const std::shared_ptr<Actor1>& b)
	{
		// Ensure both actors are valid before proceeding
		if (!a || !b) return false;

		// Calculate the vector between the centers of the two spheres
		glm::vec3 centerDiff = a->GetCenter() - b->GetCenter();

		// Compute the distance between the centers of the two spheres
		float distance = glm::length(centerDiff);

		// Retrieve the radius of the two spheres
		float radiusA = a->GetExtent().x; // Radius of the smaller sphere (Actor a)
		float radiusB = b->GetExtent().x; // Radius of the larger bounding sphere (Actor b)

		// Calculate the distance from the center of the larger sphere to the edge of the smaller sphere
		float bound = distance + radiusA;

		// Check if the smaller sphere is outside the larger sphere
		if (bound > radiusB)
		{
			// Calculate the direction of the collision (normalized vector pointing from b to a)
			glm::vec3 collisionNormal = glm::normalize(centerDiff);

			// Compute how far the smaller sphere is outside the larger sphere
			float penetrationDepth = bound - radiusB;

			// Reposition the smaller sphere inward to resolve the collision
			a->rigidB->SetLocalPosition(a->rigidB->GetLocalPosition() - collisionNormal * penetrationDepth);

			// Calculate the relative velocity between the two spheres
			glm::vec3 relativeVelocity = a->rigidB->velocity - b->rigidB->velocity;

			// Project the relative velocity onto the collision normal to calculate impact speed
			float impactSpeed = glm::dot(relativeVelocity, collisionNormal);

			// Prevent vertical motion if necessary
			a->rigidB->velocity.y = 0;

			// Apply impulse to reverse the velocity of the smaller sphere if it is moving outward
			if (impactSpeed > 0)
			{
				glm::vec3 impulse = collisionNormal * impactSpeed * 2.0f;
				a->rigidB->velocity -= impulse;
			}
			return true; // Collision occurred
		}
		return false; // No collision detected
	}

	template<typename Actor, typename Actor1>
	static void TrackPlayer(const std::shared_ptr<Actor>& _target, const std::shared_ptr<Actor1>& _seeker, float dt, float homingSpeed)
	{
		if (!_target || !_seeker)
		{
			//std::cout << "[WARNING]: _target or _seeker is nullpointer\n";
			return;
		}

		const glm::vec3& path = (dt/homingSpeed) * (_target->GetLocalPosition() - _seeker->GetLocalPosition()); 
		_seeker->SetLocalPosition(_seeker->GetLocalPosition() + path); 
		

		if (AABB(_target, _seeker))
		{
			//std::cout << "[LOG]:" << _seeker->mName << " Destroyed \n";  
			std::cout << "[LOG]:Damage taken:" << std::to_string(10) << " health is: " << _target->GetHealth() << " \n"; 
			_target->Health = _target->Health - 10;   
			_seeker->~Actor(); 

			if (_target->Health <= 0)
			{
				_target->die = true;
				return;
			}
			dt = 0;
			return;
		}
		return;
	}

	template<typename Actor, typename Actor1>
	static void ProjectileHit(const std::shared_ptr<Actor>& _projectile, const std::shared_ptr<Actor1>& _target)
	{
		if (!_projectile || !_target) 
		{
			//std::cout << "[WARNING]: _target or _seeker is nullpointer\n";
			return;
		}
		if (AABB(_projectile, _target))
		{
			_projectile->~Actor();

			//std::cout << "[LOG]:" << _seeker->mName << " Destroyed \n";  
			std::cout << "[LOG]:" << std::to_string(50) << " Damage done to:" << _target->mName<< "\n";
			_target->Health = _target->Health - 10;

			if (_target->Health <= 0)
			{
				_target->~Actor() ;
			}
			return;
		}
		return;
	}

	static bool AABBConvex(const glm::vec3& verticesPos, const glm::vec3& center, const float& sectorRadius, const float& extentRadius)
	{
		//Define max and min extent
		glm::vec3 minExtent = { center.x - extentRadius, 0.f, center.z - extentRadius };
		glm::vec3 maxExtent = { center.x + extentRadius, 0.f, center.z + extentRadius }; 

		//calculate sum of extent
		glm::vec3 sumOfExtent = (maxExtent - minExtent);    
		sumOfExtent.y = sectorRadius;  

		glm::vec3 centerDiff = verticesPos - center; 

		for (int i = 0; i < 3; i++)
		{
			// Check for no overlap
			if (glm::abs(centerDiff[i]) >= sumOfExtent[i])
			{
				return false;
			}
		}
		return true;
	}

};

