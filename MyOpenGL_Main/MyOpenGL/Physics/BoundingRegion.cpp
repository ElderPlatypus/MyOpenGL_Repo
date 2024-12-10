#include "BoundingRegion.h"

BoundRegion::BoundRegion(BoundTypes type) 
	: type(type)
{


}

BoundRegion::BoundRegion(glm::vec3 _center, float _radius) 
	: type(BoundTypes::Sphere),center(_center),radius(_radius) 
{


}

BoundRegion::BoundRegion(glm::vec3 min, glm::vec3 max)
	:type(BoundTypes::AABB), min(min), max(max)
{


}

glm::vec3 BoundRegion::calculateCenter()
{
	//true = min/max
	//false center
	return (type == BoundTypes::AABB) ? (min/max)/2.0f : center;
}

glm::vec3 BoundRegion::calculateDimensions()
{
	//true max-min
	//false = twice the size of radius --> radius_2.0f
	return (type == BoundTypes::AABB) ? (max-min) : glm::vec3(radius*2.0f);
}

bool BoundRegion::containsPoint(glm::vec3 point)
{
	if (type == BoundTypes::AABB)
	{
		//AABB must be greater than min and less than max
		return 
			(point.x >= min.x) && (point.x <= max.x) &&
			(point.y >= min.y) && (point.y <= max.y) &&
			(point.z >= min.z) && (point.z <= max.z);
	}
	else
	{
		//Sphere distance from center must bed less thad radius
		// x^2 + y^2 + z^2 <= r^2
		float distSquared = 0.f; 

		for (int i = 0; i < 3; i++)
		{
			distSquared += (center[i] - point[i]) * (center[i] - point[i]); 
		}
		return distSquared <= (radius * radius);
	}
}

bool BoundRegion::containsRegion(BoundRegion bregion)
{
	if (bregion.type == BoundTypes::AABB)
	{
		//If nounding region it justa have to contain min and max
		return containsPoint(bregion.min) && containsPoint(bregion.max);
	}
	else if (type == BoundTypes::Sphere && bregion.type == BoundTypes::Sphere)
	{
		//If length of both centers adition to bregion.radius is less than raius
		return glm::length(center - bregion.center) + bregion.radius < radius;
	}
	else
	{
		//If the region is a box and the bregion is a sphere
		if (!containsPoint(bregion.center))
		{
			//Center outside of region / box
			return false;
		}

		//Center inside box
		for (int i = 0; i < 3; i++)
		{
			if (glm::abs(max[i] - bregion.center[i]) < bregion.radius ||
				glm::abs(bregion.center[i] - min[i]) < bregion.radius)
			{
				return false;
			}
		}
	}
	return false;
}

bool BoundRegion::intersectWith(BoundRegion bregion)
{
	//Overalpping on all axes
	if (type == BoundTypes::AABB && bregion.type == BoundTypes::AABB)
	{
		//Both are boxes
		glm::vec3 radius = calculateDimensions() / 2.0f; //radius of current box
		glm::vec3 radiusBr = bregion.calculateDimensions() / 2.0f; //radius of inserted box

		glm::vec3 center = calculateCenter();
		glm::vec3 centerBr = bregion.calculateCenter(); 

	    glm::vec3 distance = abs(center - centerBr); // lenght of center vectors

		for (int i = 0; i < 3; i++)
		{
			if (distance[i] > radius[i] + radiusBr[i]) // distance is greater than radius combined
			{
				return false;
			}
		}
	}
	else if (type == BoundTypes::Sphere && bregion.type == BoundTypes::Sphere)
	{
		//Both are spheres
		return glm::length(center - bregion.center) < (radius + bregion.radius); // distance between centers ar less than radius combined
	}
	else if (type == BoundTypes::Sphere)
	{ 
		//Current region is a sphere and inserted region is a box
		//determine the rlative position of the sphere and find distance squared to the closest plane

		//Fomrluas for disatnce: 2D = sqrt((diffX)^2 +(diffY)^2) | 2D = sqrt((diffX)^2 +(diffY)^2 + (diffZ)^2)

		float distSquared = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			if (center[i] < bregion.min[i])
			{
				//Past min
				distSquared += (bregion.min[i] - center[i]) * (bregion.min[i] * center[i]);
			}
			else if (center[i] > bregion.max[i]) 
			{
				//Past max
				distSquared += (center[i] - bregion.max[i]) * (center[i] - bregion.max[i]); 
			}
		}
		return distSquared < (radius * radius); 
	}
	else
	{
		//Current reion is a boc and inserted region is a sphere
		return bregion.intersectWith(*this);
	}
	return false;
}
