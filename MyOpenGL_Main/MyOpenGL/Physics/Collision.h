#pragma once
#include "../Utility/Transform.h"
#include "../SceneFolder/Actor.h"
#include <random>

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

	static bool InvertIntersect(Actor* a, Actor* b)
	{
		if (!a->mMesh->mEnableCollision || !b->mMesh->mEnableCollision) return false;

		//Calculate the center difference and sum of the boundg boxes
		glm::vec3 centerDiff = ((a->mMesh->mCenter - b->mMesh->mCenter) - (a->mMesh->maxExtent));
		glm::vec3 boxExtent = b->mMesh->mExtent; 

		//glm::vec3 correction = (centerDiff * 0.2f) * glm::sign(a->mVelocity); 

		for (int i = 0; i < 3; i++)
		{ 
			//If difference per axis is less than sum of extent then no collision is detected
			if (glm::abs(centerDiff[i]) >= boxExtent[i])
			{

				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<float> randDirectionX(-3.f, 2.f);
				std::uniform_real_distribution<float> randDirectionY(-1.f, 3.f);
				std::uniform_real_distribution<float> randDirectionZ(-2.f, 1.f);

				glm::vec3 newDirection{ randDirectionX(gen),randDirectionY(gen),randDirectionZ(gen) };
				//Environment::gravitationalAcceleraton = newDirection; 
				a->rigidB.velocity *= -1.0f; 
				return true;
			}
		}
		//Collision detected
		return false;
	}
};

