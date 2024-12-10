#pragma once

//External Includes
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

//Includes c++
#include <algorithm>
#include <typeindex>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <vector>
 
///Components
class IComponent
{
public:
	virtual void displayComponent()
	{
		//std::cout<< "[LOG]:ComponentInterface Created \n";
	};
	virtual ~IComponent() = default;
};

template<typename T>
class ComponentArchive : IComponent
{
public:
	void displayComponent() override
	{
		std::cout << "[LOG]:ComponentArchive Type: " <<  typeid(T).name() << "\n";
	}

private:
	std::unordered_map<std::string, std::vector<std::shared_ptr<T>>> m_componentArchive; 

public:
	//Add component
	void AddComponent(const std::string& _componentType, const std::shared_ptr<T>& _component)   
	{
		if (_componentType.empty() || !_component)
		{
			throw std::runtime_error("[LOG]:Component not found or id doesnt exists.");  
		}   
		m_componentArchive[_componentType].emplace_back(_component); 
	}

	//Get component single
	std::shared_ptr<T> GetComponent(const std::string& _entityType, const std::shared_ptr<T> _component) 
	{
		//
		auto content = m_componentArchive.find(_entityType);

		if(content != m_componentArchive.end() || !content->second.empty) 
		{
			return content->second; 
		}
		return nullptr;
	}

	//Flag if component exists
	bool HasComponent(const std::string& _entityType) 
	{
		return m_componentArchive.find(_entityType) != m_componentArchive.end(); 
	}

	//Remove component
	void RemoveComponent(const std::string& _entityType)
	{
		if (!HasComponent(_entityType))
		{
			throw std::runtime_error("[ERROR]:Component type not found.");
		}
		m_componentArchive->erase(_entityType);  
	}

	//Get all components
	std::unordered_map<std::string, std::vector<std::shared_ptr<T>>> GetAllComponents() const 
	{
		return m_componentArchive;
	}
};


//TypeComponents
struct TransformComponent : public IComponent
{
	void displayComponent() override
	{
		std::cout << "[LOG]:TransformComponent";
	}
	std::vector<glm::vec3> m_pos; 
	std::vector<glm::quat> m_Rotation;
	std::vector<int> scale;
};

struct HealthComponent : public IComponent
{
	void displayComponent() override
	{
		std::cout << "[LOG]:HealthComponent";
	}
	std::vector<int> health;
};

struct DamageComponent : public IComponent
{
	void displayComponent() override
	{
		std::cout << "[LOG]:DamageComponent";
	}
	std::vector<int> damage;
};

struct PhysicsComponent : public IComponent
{
	void displayComponent() override
	{
		std::cout << "[LOG]:PhysicsComponent Created";
	}
	std::vector<bool> m_EnablePhysics;
};

struct SceneGraphComponent : public IComponent
{
	void displayComponent() override
	{
		std::cout << "[LOG]:SceneGraphComponent Created";
	}
	std::unordered_map<std::string, std::shared_ptr<Actor>> uActorMap;
};



