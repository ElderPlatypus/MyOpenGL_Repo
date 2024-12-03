#pragma once

//Full inclusion
#include "../Physics/Environment.h"

//External includes
#include <glm/gtc/random.hpp>

struct Collision
{
	template<typename Actor, typename Actor1>
	static bool AABB(const std::shared_ptr<Actor>& a, const std::shared_ptr<Actor1>& b)
	{
		//Check if actors have collision enables
		if (!a || !b) return false;
		
		//Calculate the center difference and sum of the boundg boxes
		glm::vec3 centerDiff = glm::abs(a->GetCenter() - b->GetCenter()); 
		glm::vec3 sumOfExtent = a->GetExtent() + b->GetExtent();  
	
		for (int i = 0; i < 3; i++)
		{
			// Check for no overlap
			if (centerDiff[i] >= sumOfExtent[i]) 
			{
				return false;
			}
		}
		return true;
	}

	template<typename Actor, typename Actor1>
	static bool AABBInverse(const std::shared_ptr<Actor>& a, const std::shared_ptr<Actor1>& b)
	{
		if (!a || !b) return false;

		//Calculate the center difference and sum of the bounding boxes
		const glm::vec3& centerDiff = glm::abs(b->GetCenter() - (a->GetCenter() + a->GetExtent()));
		const glm::vec3& boxExtent = glm::abs(b->GetCenter() - b->GetExtent());  //center to mesh  

		glm::vec3 newDir1 = glm::linearRand(glm::vec3(-10.f), glm::vec3(10.f));
		a->rigidB->acceleration = glm::vec3{newDir1.x,Environment::gravitationalAcceleraton.y,newDir1.z};  

		
		if (centerDiff.x >= boxExtent.x)
		{
			/*std::cout << centerDiff.x << " Center diff X \n";
			std::cout << boxExtent.x << " Box Extent X  \n"; 
			std::cout << "\n";*/

			a->rigidB->velocity.x *= -1; 
			return true;
		}
		else if (centerDiff.y >= boxExtent.y)
		{
		/*	std::cout << centerDiff.y << " Center diff Y \n";
			std::cout << boxExtent.y << " Box Extent Y  \n";
			std::cout << "\n";*/

			a->rigidB->velocity.y *= -1;
			return true;

		}
		else if (centerDiff.z >= boxExtent.z)
		{
			/*std::cout << centerDiff.z << " Center diff Z \n";
			std::cout << boxExtent.z << " Box Extent Z  \n";
			std::cout << "\n";*/

			a->rigidB->velocity.z *= -1;
			return true;

		}
		return false;
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

	template<typename Actor, typename Actor1>
	static bool BallBall(const std::shared_ptr<Actor>& a, const std::shared_ptr<Actor1>& b) 
	{
		//Check if actors have collision enables
		if (!a || !b) return false;

		//Calculate the center difference and sum of the boundg boxes
		glm::vec3 centerDiff = glm::abs(a->GetCenter() - b->GetCenter());
		float distance = glm::length(centerDiff);

		float combinedRadius = a->GetExtent().x + b->GetExtent().x;

		if (distance < combinedRadius)
		{

			glm::vec3 collisionNormal = glm::normalize(centerDiff);

			float penetrationDepth = combinedRadius - distance; 

			a->SetLocalPosition(a->GetLocalPosition() + collisionNormal * (penetrationDepth * 0.5f)); 
			b->SetLocalPosition(b->GetLocalPosition() - collisionNormal * (penetrationDepth * 0.5f)); 

			glm::vec3 relativeVelocity = a->rigidB->velocity - b->rigidB->velocity;
			float impactSpeed = glm::dot(relativeVelocity, collisionNormal);

			if (impactSpeed < 0)
			{
				glm::vec3 impulse = collisionNormal * impactSpeed * 2.0f;
				a->rigidB->velocity -= impulse;
				b->rigidB->velocity += impulse;
			}
			return true;
		}
		return false; 
	}
};

