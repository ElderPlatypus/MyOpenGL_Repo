#pragma once
#include "Transform.h"
#include "../SceneFolder/Actor.h"


class Collision
{
public:

	static bool Intersect(Actor* a, Actor* b)
	{
		//Check if actors have collision enables
		if (!a->mMesh->mEnableCollision || !b->mMesh->mEnableCollision) return false;

		//Calculate the center difference and sum of the boundg boxes
		glm::vec3 centerDiff =  a->mMesh->mCenter - b->mMesh->mCenter; 
		glm::vec3 sumOfExtent = a->mMesh->mExtent + b->mMesh->mExtent;

		const int vertexPlacement = 3;
		//CHecking for collison for all axis
		for (int i = 0; i < 3; i++)
		{
			//Check for no overlap
			if (glm::abs(centerDiff[i]) >= sumOfExtent[i])
			{
				//std::cout << "Center Diff Length : " << std::abs(centerDiff[i]) << std::endl;
				return false;
			}
		}

		//Collision detected
		return true;
	}

	static bool InvertIntersect(Actor* a, Actor* b, float delta)
	{
		if (!a->mMesh->mEnableCollision || !b->mMesh->mEnableCollision) return false;

		//Calculate the center difference and sum of the boundg boxes
		glm::vec3 centerDiff = (a->mMesh->mCenter - a->mMesh->mExtent*0.5f) - b->mMesh->mCenter;

		glm::vec3 boxExtent = b->mMesh->mExtent; 
		glm::vec3 correction = (centerDiff * 0.2f) * glm::sign(a->mVelocity); 
		 
		for (int i = 0; i < 3; i++)
		{ 
			//If difference per axis is less than sum of extent then no collision is detected
			if (glm::abs(centerDiff[i]) >= boxExtent[i])
			{
			/*	std::cout << "Center Diff Length : " << std::abs(centerDiff[i]) << std::endl;
				std::cout << "Extent Length : " << boxExtent[i] << std::endl;*/
				//std::cout << "Inverse Collision" << std::endl;
				if (a->mVelocity != glm::vec3(0.f))
				{
				  //a->mMesh->SetLocalPosition(a->mMesh->GetLocalPosition()-correction);
					a->mMovementSpeed *= -1;
				}
				//a->mMesh->GetLocalPosition() * 0.f * delta;
				return true;
			}
		}
		//Collision detected
		return false;
	}
};

