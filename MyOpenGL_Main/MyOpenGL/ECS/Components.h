#pragma once

#define GLM_ENABLE_EXPERIMENTAL
//External
#include <typeindex>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

//classes

#include "../SceneFolder/Actor.h"
#include "Entity.h" 
 
///Components
class IComponent
{
public:
	virtual void displayComponent()
	{
		//std::cout<< "[LOG]:ComponentInterface Created \n";
	};
	virtual ~IComponent()
	{
		//std::cout << "[LOG]:ComponentInterface Destroyed \n";
	};
};

template<typename T>
class ComponentArchive : IComponent
{
public:
	void displayComponent() override
	{
		std::cout << "[LOG]:ComponentArchive Type: " <<  typeid(T).name() << "\n";
	}
	/*~ComponentArchive() override
	{
		std::cout << "[LOG]:ComponentArchive Destroyed \n";
	}*/

private:
	std::unordered_map<int, std::vector<std::shared_ptr<T>>> m_componentArchive;

public:
	void AddComponent(const int& _entityId, const std::shared_ptr<T>& _component)
	{
		if (_entityId < 0 || !_component) 
		{
			throw std::runtime_error("[LOG:]Component not found or id doesnt exists."); 
		}
		m_componentArchive[_entityId].emplace_back((_component)); 
	}

	std::shared_ptr<T> GetComponent(const int& _entityId)
	{
		try
		{
			return m_componentArchive[_entityId].front();
		}
		catch (const std::exception& e) {
			// Catch any exception thrown during the execution
			// of divide function
			std::cerr << "Exception caught: " << e.what() << std::endl;
			return nullptr;
		}
	}

	bool HasComponent(const int& _entityId)
	{
		return m_componentArchive.find(_entityId) != m_componentArchive.end();
	}

	void RemoveComponent(const int& _entityId)
	{
		if (!HasComponent(_entityId))
		{
			throw std::runtime_error("[ERROR]:Component not found for the given entity ID.");
		}
		m_componentArchive.erase(_entityId);
	}

	std::unordered_map<int, std::vector<std::shared_ptr<T>>> GetAllComponents() const 
	{
		return m_componentArchive;
	}


};


//TypeComponents
struct ActorComponent : public IComponent
{
	void displayComponent() override
	{
		std::cout << "ActorComponent:";
	}
    std::vector<std::shared_ptr<Actor>> actors;
	std::vector<std::vector<std::shared_ptr<Actor>>> spawn;
};

struct PlayerComponent : public IComponent
{
	void displayComponent() override
	{
		std::cout << "PlayerComponent";
	}
	std::vector<std::shared_ptr<Actor>> actors;
};


struct TransformComponent : public IComponent
{
	void displayComponent() override
	{
		std::cout << "TransformComponent";
	}
	std::vector<float> X;
	std::vector<float> Y;
	std::vector<float> Z;
	std::vector<glm::vec3> m_pos; 
	glm::quat m_Rotation = { 0,0,0,0 };
	int _Size = 0;
};

struct HealthComponent : public IComponent
{
	void displayComponent() override
	{
		std::cout << "HealthComponent";
	}
	std::vector<int> health;
};

struct DamageComponent : public IComponent
{
	void displayComponent() override
	{
		std::cout << "DamageComponent";
	}
	std::vector<int> damage;
};

struct PhysicsComponent : public IComponent
{
	void displayComponent() override
	{
		std::cout << "[LOG:]PhysicsComponent Created";
	}
};


