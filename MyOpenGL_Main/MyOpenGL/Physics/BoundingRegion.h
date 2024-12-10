#pragma once
//Includes c++
#include <vector>


//Full Inclusion
#include <glm/glm.hpp>
#include "../Utility/EnumArchive.h"

//Forward Decleration


class BoundRegion
{
public:
	BoundTypes type;

	//Object values
	glm::vec3 center{};
	float radius = 0.0f;

	//Bounding type value
	glm::vec3 min{};
	glm::vec3 max{};

	///Constructor

	//Init with type
	BoundRegion(BoundTypes type);

	//Init as Sphere
	BoundRegion(glm::vec3 center, float radius);

	//Init as AABB
	BoundRegion(glm::vec3 min, glm::vec3 max);

	///Calculations

	//Center
	glm::vec3 calculateCenter();

	//Calculate Dimensions
	glm::vec3 calculateDimensions();

	///Testing

	//Deteermine if point exists in region completly
	bool containsPoint(glm::vec3 point);

	bool containsRegion(BoundRegion b_region);

	bool intersectWith(BoundRegion b_region);

};
