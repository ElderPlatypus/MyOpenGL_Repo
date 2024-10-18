#pragma once
#include "../Utility/Transform.h"
#include "../SceneFolder/Actor.h"
#include <glm/gtc/random.hpp>

class Collision
{
public:
	static bool AABB(const std::shared_ptr<Actor>& a, const std::shared_ptr<Actor>& b)
	{
		//Check if actors have collision enables
		if (!a->mMesh->mEnableAABBCollision || !b->mMesh->mEnableAABBCollision) return false;
		
		//Calculate the center difference and sum of the boundg boxes
		glm::vec3 centerDiff =  a->mMesh->mCenter - b->mMesh->mCenter; 
		glm::vec3 sumOfExtent = a->mMesh->mExtent + b->mMesh->mExtent;
	
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

	static bool AABBInverse(Actor* a, Actor* b,float dt)
	{
		if (!a->mMesh->mEnableInverseAABB || !b->mMesh->mEnableInverseAABB) return false;

		//Calculate the center difference and sum of the bounding boxes
		const glm::vec3& centerDiff = glm::abs(b->mMesh->mCenter - (a->mMesh->mCenter + a->mMesh->mExtent));
		const glm::vec3& boxExtent = glm::abs(b->mMesh->mCenter - b->mMesh->mExtent);  //center to mesh 

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

	static void TrackPlayer(const std::shared_ptr<Actor>& _target, const std::shared_ptr<Actor>& _seeker, float dt)
	{
		if (!_target || !_seeker || _target == _seeker)
		{
			throw std::runtime_error("[WARNING]: _target or _seeker is nullpointer");
			return;
		}

		glm::vec3 path = (_target->mMesh->GetLocalPosition() - _seeker->mMesh->GetLocalPosition());
		
		//_seeker->mMesh->SetLocalPosition(path);

		if (AABB(_target, _seeker))
		{
			//std::cout << "[LOG]:" << _seeker->mName << " Destroyed \n";
			
		}
	}
};

