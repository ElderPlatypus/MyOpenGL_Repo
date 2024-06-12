#pragma once
#include "Transform.h"
#include "../SceneFolder/Actor.h"


class Collision
{
public:

	int AABB(Actor* A, Actor* B)
	{
		if (A->GetLocalPosition() < B->GetLocalPosition().x || A->GetLocalPosition().x > B->GetLocalPosition().x) return 0;
		std::cout << "x-collison\n" << std::endl;

		if (A->GetLocalPosition().y < B->GetLocalPosition().y || A->GetLocalPosition().y > B->GetLocalPosition().y) return 0; 
		std::cout << "y-collison\n" << std::endl;

		if (A->GetLocalPosition().z < B->GetLocalPosition().z || A->GetLocalPosition().z > B->GetLocalPosition().z) return 0;
		std::cout << "z-collison\n" << std::endl;


		return 1;
	};


};
