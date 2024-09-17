#pragma once
#include "../Utility/Transform.h"
#include "../SceneFolder/Actor.h"
#include <glm/gtc/random.hpp>

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

		//CHecking for collison for all axis
		for (int i = 0; i < 3; i++)
		{
			//Check for no overlap
			if (glm::abs(centerDiff[i]) <= sumOfExtent[i])
			{
				glm::vec3 pushDirection = glm::normalize(centerDiff);
				b->mMesh->SetLocalPosition(b->mMesh->GetLocalPosition() - pushDirection * (sumOfExtent.x - glm::abs(centerDiff.x)));
				//std::cout << "Center Diff Length : " << std::abs(centerDiff[i]) << std::endl;
				return true;
			}
		}
		return true;
	}

	static bool InvertIntersect(Actor* a, Actor* b,float dt)
	{
		if (!a->mMesh->mEnableCollision || !b->mMesh->mEnableCollision) return false;

		//Calculate the center difference and sum of the bounding boxes
		const glm::vec3& centerDiff = glm::abs(glm::abs(a->mMesh->mCenter - a->mMesh->mExtent) - b->mMesh->mCenter);  
		const glm::vec3& boxExtent = glm::abs(b->mMesh->mCenter - b->mMesh->mExtent);  //center to mesh

		glm::vec3 newDir1 = glm::linearRand(glm::vec3(-10.f), glm::vec3(10.f));
		a->rigidB.acceleration = glm::vec3{newDir1.x,Environment::gravitationalAcceleraton.y,newDir1.z};  

		const glm::vec3& test = glm::abs(a->mMesh->mExtent - a->mMesh->mCenter);  

		for (int i = 0; i < 3; i++)
		{ 
			//If difference per axis is less than sum of extent then no collision is detected
			if (test[i] >= boxExtent[i]) 
			{
				switch (i)
				{
				case 0:
						//std::cout <<  "Switch X \n";
						a->rigidB.velocity.x  *= -1.0f;
						return true;

				case 1:
					    //Collision detecteds
					    if (a->rigidB.velocity.y < 0)
					    {
					    	//std::cout <<  "Switch Y \n";
					    	a->rigidB.velocity.y *= -1.0f;
					    }
					    return true;

				case 2:
						//std::cout <<  "Switch Z \n";
						a->rigidB.velocity.z *= -1;
						return true;
				}
			}
		}
		return false;
	
	}

};

