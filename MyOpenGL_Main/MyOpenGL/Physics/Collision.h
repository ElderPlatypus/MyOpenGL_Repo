#pragma once
#include "../MathLib/Transform.h"
#include "../SceneFolder/Actor.h"
#include <glm/gtc/random.hpp>

class Collision
{
public:
	static bool AABB(const std::shared_ptr<Actor>& a, const std::shared_ptr<Actor>& b)
	{
		//Check if actors have collision enables
		if (!a || !b || !a->mEnableAABBCollision || !b->mEnableAABBCollision) return false;
		
		//Calculate the center difference and sum of the boundg boxes
		glm::vec3 centerDiff =  a->GetCenter() - b->GetCenter();
		glm::vec3 sumOfExtent = a->GetExtent() + b->GetExtent();
	
		for (int i = 0; i < 3; i++)
		{
			// Check for no overlap
			if (glm::abs(centerDiff[i]) >= sumOfExtent[i])
			{
				return false;
			}
		}

		glm::vec3 pushDirection = glm::normalize(centerDiff);

		// Resolve collision along the axes
		for (int i = 0; i < 3; i++)
		{
			if (glm::abs(centerDiff[i]) <= sumOfExtent[i])
			{
				//b->mMesh->SetLocalPosition(b->mMesh->GetLocalPosition() - pushDirection * (sumOfExtent[i] - glm::abs(centerDiff[i])));
				//std::cout << "Collision detected on axis " << i << "\n";
				return true;
			}
		} 

		return false;
	}

	static bool AABBInverse(const std::shared_ptr<Actor>& a, const std::shared_ptr<Actor>& b) 
	{
		if (!a->mEnableInverseAABB || !b->mEnableInverseAABB) return false;

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

	static void TrackPlayer(const std::shared_ptr<Actor>& _target, const std::shared_ptr<Actor>& _seeker, float dt, float homingSpeed)
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
			_seeker->DeleteSpawnvector_single(_seeker);

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

	static void ProjectileHit(const std::shared_ptr<Actor>& _projectile, const std::shared_ptr<Actor>& _target)
	{
		if (!_projectile || !_target) 
		{
			//std::cout << "[WARNING]: _target or _seeker is nullpointer\n";
			return;
		}
		if (AABB(_projectile, _target))
		{
			_projectile->DeleteSpawnvector_single(_projectile); 

			//std::cout << "[LOG]:" << _seeker->mName << " Destroyed \n";  
			std::cout << "[LOG]:" << std::to_string(50) << " Damage done to:" << _target->mName<< "\n";
			_target->Health = _target->Health - 10;

			if (_target->Health <= 0)
			{
				_target->DeleteSpawnvector_single(_target);  
			}
			return;
		}
		return;
	}



};

