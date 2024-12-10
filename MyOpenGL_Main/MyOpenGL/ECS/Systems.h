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
	~ISystem() = default;
};

class ScemeGraphSystem : public ISystem
{
	std::unordered_map<std::string, std::vector<std::shared_ptr<SceneGraphComponent>>> m_SceneGraph;

public:
	//void Update() override
};