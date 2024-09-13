#pragma once
#include "Transform.h"
#include "../SceneFolder/Actor.h"


class Collision
{
public:

	static bool Intersect(Actor* a, Actor* b)
	{

		if (a->mMesh->mEnableCollision == true && b->mMesh->mEnableCollision == true)
		{
			//Center difference
			glm::vec3 diff{ a->mMesh->mCenter - b->mMesh->mCenter };

			//Sum of extent
			glm::vec3 sumExtent{ a->mMesh->mExtent + b->mMesh->mExtent };

			for (int i = 0; i < 3; i++)
			{
				//If difference per axis is less than sum of extent then no collision is detected
				if (abs(diff[i]) >= sumExtent[i]) return false;
			}
			//std::cout << "Collison Detected" << std::endl;
			return true;
		}
		else
		{
			return false;
		}
	}

	//static bool InvertIntersect(Actor* a, Actor* b)
	//{

	//	if (a->mMesh->mEnableCollision == true && b->mMesh->mEnableCollision == true)
	//	{
	//		//Center difference
	//		glm::vec3 diff{ a->mMesh->mCenter - b->mMesh->mCenter };

	//		//Sum of extent
	//		glm::vec3 wallDiff{ b->mMesh->mExtent - b->mMesh->mCenter };
	//		glm::vec3 wallDiff2{ a->mMesh->mExtent - a->mMesh->mCenter };
	//		auto a = diff + wallDiff2;

	//		for (int i = 0; i < 3; i++)
	//		{
	//			//If difference per axis is less than sum of extent then no collision is detected
	//			if (abs(a[i]) <= abs(wallDiff[i])) return false;
	//		}
	//		//std::cout << "Collison Detected" << std::endl;
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}
};

