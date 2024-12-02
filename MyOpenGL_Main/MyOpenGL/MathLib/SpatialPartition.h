#pragma once

//Glm include
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <algorithm>

//Includes c++
#include <vector>
#include <memory>
#include <utility>
#include <unordered_map>
#include <iostream>
#include <string>
#include <windows.h>


//Includes
#include "../Definitions.h"
#include "../MathLib/Vertex.h"
#include "../SceneFolder/Actor.h"
#include "../MathLib/Formulas.h"

//Forward Declearing


struct SpatialGrid
{
	//Grid cell 
	using Cell = std::vector<std::shared_ptr<Actor>>;

	//Hash map for storing grid cells
	std::unordered_map < glm::ivec3, Cell, MathLib::GridHash> grid;

	//Grid cell size
	float cellSize;

	//Constructor
	SpatialGrid(float _cellSize) : cellSize(_cellSize) {};

	//Calulating grid cell coordinates for positioning
	glm::ivec3 GetCellCoordinates(const glm::vec3& position)
	{
		//Using std::flor to round to lowest integer from current value
		return glm::ivec3(std::floor(position.x / cellSize), std::floorf(position.y / cellSize), std::floor(position.z / cellSize));  
	}

	//Populating the grid with actors for the calculated grids
	void InsertActor(const std::shared_ptr<Actor>& actor)
	{
		glm::ivec3 cell = GetCellCoordinates(actor->GetCenter());
		grid[cell].push_back(actor);
	}

	//Clear grid for new use
	void Clear()
	{
		grid.clear();
	}
};