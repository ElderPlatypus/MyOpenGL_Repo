#pragma once

//Includes c++
#include <typeindex>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <stdexcept>

//Full Incluson
#include "Components.h"

///_________________________System Interface Class___________________________
struct ISystem
{
	virtual void Update()  
	{
		std::cout << "[LOG]:ISystem Updating \n";
	}
};

