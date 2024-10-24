#pragma once

//External
#include <typeindex>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <stdexcept>

//classes
#include "Components.h"
#include "Entity.h"
#include "../SceneFolder/Actor.h"

///_________________________System Interface Class___________________________
struct ISystem
{
	virtual void Update(const std::vector<std::shared_ptr<Entity>>& _entities)
	{
		std::cout << "[LOG]:ISystem Updating \n";
	}
};

class MovementSystem : public ISystem
{
private:
	std::shared_ptr<ComponentArchive<TransformComponent>> m_transformManager; 

public:
	MovementSystem(const std::shared_ptr<ComponentArchive<TransformComponent>>& _transformManager) 
	{
		this->m_transformManager = _transformManager;
	}

	void Update(const std::vector<std::shared_ptr<Entity>>& _entities) override
	{
		std::cout << "[LOG]:MovementSystem Updating \n";
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_transformManager->HasComponent(entity->GetId()))
			{
				const std::shared_ptr<TransformComponent>& position = m_transformManager->GetComponent(entity->GetId());
				m_transformManager->AddComponent(entity->GetId(), position);
			}
			else
			{
				throw std::runtime_error("[WARNING]:Entity Id or TransformComponent not found \n");
			}
		}
		std::cout << "\n\n";
	}
};


class DamageSystem : public ISystem
{
private:
	std::shared_ptr<ComponentArchive<DamageComponent>> m_damageManager;

	void DisplayAllComponents(const std::vector<std::shared_ptr<Entity>>& _entities) const
	{
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_damageManager->HasComponent(entity->GetId()))
			{
				for (const auto& [id, component] : m_damageManager->GetAllComponents())
				{
					int counter = 0;
					m_damageManager->displayComponent();
					for (const auto& it : component)
					{
						std::cout << "[LOG]:Index:" + std::to_string(counter) + " = "; it->displayComponent();  std::cout << "\n";
						counter++;
					}
				}
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
	}
public:
	DamageSystem(const std::shared_ptr<ComponentArchive<DamageComponent>>& _m_damageManager)
	{
		if (!_m_damageManager) return;
		this->m_damageManager = _m_damageManager;
	
		 
	}

	void Update(const std::vector<std::shared_ptr<Entity>>& _entities) override
	{
		if (_entities.empty()) return;

		std::cout << "[LOG]:DamageSystem Updating \n";
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_damageManager->HasComponent(entity->GetId()))
			{

				const std::shared_ptr<DamageComponent>& damage = m_damageManager->GetComponent(entity->GetId());
				m_damageManager->AddComponent(entity->GetId(), damage); 
				DisplayAllComponents(_entities);

			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
		std::cout << "\n";
		std::cout << "\n";
	}


};

class PlayerSystem : public ISystem
{
private:
	std::shared_ptr<ComponentArchive<PlayerComponent>> m_playerManager;

	void DisplayAllComponents(const std::vector<std::shared_ptr<Entity>>& _entities) const
	{
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_playerManager->HasComponent(entity->GetId()))
			{
				for (const auto& [id, component] : m_playerManager->GetAllComponents())
				{
					int counter = 0;
					
					for (const auto& it : component)
					{
						std::cout << "[LOG]:Index:" + std::to_string(counter) + " = "; it->displayComponent();  std::cout << "\n";
						counter++;
					}
				}
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
	}

public:
	//Struct and Update override
	PlayerSystem(const std::shared_ptr<ComponentArchive<PlayerComponent>>& _m_playerManager)
	{
		if (!_m_playerManager) return;
		this->m_playerManager = _m_playerManager;
	}
    
	void Update(const std::vector<std::shared_ptr<Entity>>& _entities) override
	{
		if (_entities.empty()) return;

		std::cout << "[LOG]:playerSystem Updating \n";
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_playerManager->HasComponent(entity->GetId()))
			{
		        
				const std::shared_ptr<PlayerComponent>& player = m_playerManager->GetComponent(entity->GetId());
				for (const auto& player : player->actors)
				{
					m_playerManager->GetComponent(entity->GetId())->actors[entity->GetId()]->isPlayer = true;

				}
				m_playerManager->AddComponent(entity->GetId(), player); 
				DisplayAllComponents(_entities);
				
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
		std::cout << "\n";
	}


	//Render Logic
	bool UseTexture(const std::vector<std::shared_ptr<Entity>>& _entities, const int& id)
	{
		if (_entities.empty()) return false;

		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			for (const auto& [id, component] : m_playerManager->GetAllComponents())
			{
				int counter = 0;
				m_playerManager->displayComponent();
				for (const auto& it : component)
				{
					counter++;
					std::cout << "[LOG]:Adding texture to component:" + std::to_string(counter) + ""; it->actors[id]->UseTexBool(true);  std::cout << "\n";
					
				}
			}
		}
		std::cout << "\n";
		return false;
	}

	void SetShader(const std::vector<std::shared_ptr<Entity>>& _entities, const std::shared_ptr<Shader>& shader) const 
	{
		if (_entities.empty()) return;

		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			for (const auto& [id, component] : m_playerManager->GetAllComponents())
			{
				int counter = 0;
				m_playerManager->displayComponent(); 
				for (const auto& it : component)
				{
					counter++;
					std::cout << "[LOG]:Adding Shader to component:" + std::to_string(counter) + ""; it->actors[id]->SetShader(shader);  std::cout << "\n";
					
				}
			}
		}
		std::cout << "\n";
		return;
	}

	
	//Get Player
	std::shared_ptr<Actor> GetPlayer(const std::vector<std::shared_ptr<Entity>>& _entities)
	{
		if (_entities.empty()) return nullptr;

		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_playerManager->HasComponent(entity->GetId()))
			{
				  return m_playerManager->GetComponent(entity->GetId())->actors[entity->GetId()];
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
				return nullptr;
			}
		}
		return nullptr;
	}

	//Camera Logic
	bool AttachToCamera(const std::vector<std::shared_ptr<Entity>>& _entities, const int& id)
	{
		if (_entities.empty()) return false;

		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_playerManager->HasComponent(id)) 
			{
				return m_playerManager->GetComponent(entity->GetId())->actors[entity->GetId()]->mAttachToActor = true; 
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
				return false;
			}
		}
		return false;
	}

	std::shared_ptr<Actor>& Shoot()
	{

	}

	//Redner Logic
	void DrawEntity(const std::vector<std::shared_ptr<Entity>>& _entities, float dt) const 
	{
		if (_entities.empty()) return;
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			for (const auto& [id, component] : m_playerManager->GetAllComponents())
			{
				int counter = 0;
				for (const auto& it : component)
				{
					counter++;
					it->actors[id]->UpdateActors(dt);

				}
			}
		}
		return;
	}
};

class ActorSystem : public ISystem
{
private:
	std::shared_ptr<ComponentArchive<ActorComponent>> m_actorManager;

	void DisplayAllComponents(const std::vector<std::shared_ptr<Entity>>& _entities) const
	{
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_actorManager->HasComponent(entity->GetId()))
			{
				for (const auto& [id, component] : m_actorManager->GetAllComponents())
				{
					int counter = 0;
					m_actorManager->displayComponent();
					for (const auto& it : component)
					{
						std::cout << "[LOG]:Index:" + std::to_string(counter) + " = "; it->displayComponent();  std::cout << "\n";
						counter++;
					}
				}
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
	}

public:
	//Struct and Update override
	ActorSystem(const std::shared_ptr<ComponentArchive<ActorComponent>>& _m_actorManager)
	{
		if (!_m_actorManager) return;
		this->m_actorManager = _m_actorManager;
	}
    
	void Update(const std::vector<std::shared_ptr<Entity>>& _entities) override
	{
		if (_entities.empty()) return;

		std::cout << "[LOG]:actorSystem Updating \n";
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_actorManager->HasComponent(entity->GetId()))
			{
				m_actorManager->displayComponent();
				const std::shared_ptr<ActorComponent>& actor = m_actorManager->GetComponent(entity->GetId());
				for (const auto& actor : actor->actors)
				{
					m_actorManager->GetComponent(entity->GetId())->actors[entity->GetId()]->isActor = true;

				}
				m_actorManager->AddComponent(entity->GetId(), actor); 
				DisplayAllComponents(_entities);
				
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
		std::cout << "\n";
	}


	//Render Logic
	bool UseTexture(const std::vector<std::shared_ptr<Entity>>& _entities, const int& id)
	{
		if (_entities.empty()) return false;

		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			for (const auto& [id, component] : m_actorManager->GetAllComponents())
			{
				int counter = 0;
				m_actorManager->displayComponent();
				for (const auto& it : component)
				{
					counter++;
					std::cout << "[LOG]:Adding texture to component:" + std::to_string(counter) + ""; it->actors[id]->UseTexBool(true);  std::cout << "\n";
					
				}
			}
		}
		std::cout << "\n";
		return false;
	}

	void SetShader(const std::vector<std::shared_ptr<Entity>>& _entities, const std::shared_ptr<Shader>& shader) const 
	{
		if (_entities.empty()) return;

		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			for (const auto& [id, component] : m_actorManager->GetAllComponents())
			{
				int counter = 0;
				m_actorManager->displayComponent(); 
				for (const auto& it : component)
				{
					counter++;
					std::cout << "[LOG]:Adding Shader to component:" + std::to_string(counter) + ""; it->actors[id]->SetShader(shader);  std::cout << "\n";
					
				}
			}
		}
		std::cout << "\n";
		return;
	}

	
	//Get actor
	std::shared_ptr<Actor> Getactor(const std::vector<std::shared_ptr<Entity>>& _entities)
	{
		if (_entities.empty()) return nullptr;

		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_actorManager->HasComponent(entity->GetId()))
			{
				  return m_actorManager->GetComponent(entity->GetId())->actors[entity->GetId()];
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
				return nullptr;
			}
		}
		return nullptr;
	}

	//Camera Logic
	bool AttachToCamera(const std::vector<std::shared_ptr<Entity>>& _entities, const int& id)
	{
		if (_entities.empty()) return false;

		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			if (m_actorManager->HasComponent(id)) 
			{
				return m_actorManager->GetComponent(entity->GetId())->actors[entity->GetId()]->mAttachToActor = true; 
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
				return false;
			}
		}
		return false;
	}

	std::shared_ptr<Actor>& Shoot()
	{

	}

	//Redner Logic
	void DrawEntity(const std::vector<std::shared_ptr<Entity>>& _entities, float dt) const 
	{
		if (_entities.empty()) return;
		for (const std::shared_ptr<Entity>& entity : _entities)
		{
			for (const auto& [id, component] : m_actorManager->GetAllComponents())
			{
				int counter = 0;
				for (const auto& it : component)
				{
					counter++;
					it->actors[id]->UpdateActors(dt);

				}
			}
		}
		return;
	}
};