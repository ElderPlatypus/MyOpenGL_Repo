#pragma once
#include "Transform.h"
#include "../SceneFolder/Actor.h"


class Collision
{
public:

	///Notice GetPos er vektorene ikke posisjonen til vertex!!!!!!!!!!!!

	static bool Intersect(Actor* a, Actor* b) 
	{

		//Center differnce
		glm::vec3 diff{ a->mCenter - b->mCenter };

		//Sum of extent
		glm::vec3 sumExtent{ a->mExtent + b->mExtent };

		for (int i = 0; i < 3; i++)
		{
			//If difference per axis is less than sum of extent then no collision is detected
			if (abs(diff[i]) >= sumExtent[i]) return false;
		}
		std::cout << "Collison Detected" << std::endl;


		return true;
	}

	

};

//glm::vec3 distance{ abs(A->GetLocalPosition().x - B->GetLocalPosition().x), abs(A->GetLocalPosition().y - B->GetLocalPosition().y), abs(A->GetLocalPosition().z - B->GetLocalPosition().z) };
////distanceX = abs(A->GetLocalPosition().x - B->GetLocalPosition().x);
//if (distance.x < A->GetLocalPosition().x + A->GetLocalPosition().length() && distance.x < B->GetLocalPosition().x + A->GetLocalPosition().length())
//{
//	std::cout << "Collison X " << A->GetTransform().GetPosition().x << std::endl;
//	std::cout << "Collison Y " << A->GetTransform().GetPosition().y << std::endl;
//	std::cout << "Collison Z " << A->GetTransform().GetPosition().z << std::endl;
//}