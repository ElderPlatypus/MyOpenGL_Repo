#pragma once

//External
#include <typeindex>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <stdexcept>

//classes
#include "Components.h"
#include "../SceneFolder/Actor.h"

///_________________________System Interface Class___________________________
struct ISystem
{
	virtual void Update()  
	{
		std::cout << "[LOG]:ISystem Updating \n";
	}
};

