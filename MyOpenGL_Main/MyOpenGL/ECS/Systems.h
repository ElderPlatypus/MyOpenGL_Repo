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
	virtual void Update(const std::shared_ptr<Actor>& _entity)
	{
		std::cout << "[LOG]:ISystem Updating \n";
	}

	virtual void Update(const std::unordered_map<std::shared_ptr<std::string>,std::shared_ptr<Actor>>& _entities) 
	{
		std::cout << "[LOG]:ISystem Updating \n";
	}
};

class ActorSystem : public ISystem
{
private:
	std::shared_ptr<ComponentArchive<ActorComponent>> m_actorManager;

	void DisplayAllComponents(const std::shared_ptr<Actor>& _entity) const 
	{
		
		if (m_actorManager->HasComponent(_entity->mName))  
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

public:
	//Struct and Update override
	ActorSystem(const std::shared_ptr<ComponentArchive<ActorComponent>>& _m_actorManager)
	{
		if (!_m_actorManager) return;
		this->m_actorManager = _m_actorManager;
	}
    
	/*void Update(std::unordered_map<std::shared_ptr<std::string>, std::shared_ptr<Actor>>& _entities) override  
	{
		if (_entities.empty()) return;

		std::cout << "[LOG]:actorSystem Updating \n";
		for (const auto& entity : _entities) 
		{
			if (m_actorManager->HasComponent(entity->mId))
			{
				m_actorManager->displayComponent();
				const std::shared_ptr<ActorComponent>& actor = m_actorManager->GetComponent(entity->mId);
				for (const auto& actor : actor->actors)
				{
					m_actorManager->GetComponent(entity->mId)->actors[entity->mId]->isActor = true;

				}
				m_actorManager->AddComponent(entity->mName, actor);  
				DisplayAllComponents(_entities);
				
			}
			else
			{
				std::cout << "[WARNING]:Entity Id or Component not found \n";
			}
		}
		std::cout << "\n";
	}*/


	////Render Logic
	//bool UseTexture(const std::vector<std::shared_ptr<Actor>>& _entities, const int& id)
	//{
	//	if (_entities.empty()) return false;

	//	for (const std::shared_ptr<Actor>& entity : _entities)
	//	{
	//		for (const auto& [id, component] : m_actorManager->GetAllComponents())
	//		{
	//			int counter = 0;
	//			m_actorManager->displayComponent();
	//			for (const auto& it : component)
	//			{
	//				counter++;
	//				std::cout << "[LOG]:Adding texture to component:" + std::to_string(counter) + ""; it->actors[id]->UseTexBool(true);  std::cout << "\n";
	//				
	//			}
	//		}
	//	}
	//	std::cout << "\n";
	//	return false;
	//}

	//void SetShader(const std::vector<std::shared_ptr<Actor>>& _entities, const std::shared_ptr<Shader>& shader) const
	//{
	//	if (_entities.empty()) return;

	//	for (const std::shared_ptr<Actor>& entity : _entities)
	//	{
	//		for (const auto& [id, component] : m_actorManager->GetAllComponents())
	//		{
	//			int counter = 0;
	//			m_actorManager->displayComponent(); 
	//			for (const auto& it : component)
	//			{
	//				counter++;
	//				std::cout << "[LOG]:Adding Shader to component:" + std::to_string(counter) + ""; it->actors[id]->SetShader(shader);  std::cout << "\n";
	//				
	//			}
	//		}
	//	}
	//	std::cout << "\n";
	//	return;
	//}

	
	////Get actor
	//std::shared_ptr<Actor> Getactor(const std::vector<std::shared_ptr<Actor>>& _entities)
	//{
	//	if (_entities.empty()) return nullptr;

	//	for (const std::shared_ptr<Actor>& entity : _entities)
	//	{
	//		if (m_actorManager->HasComponent(entity->mId))
	//		{
	//			  return m_actorManager->GetComponent(entity->mId)->actors[entity->mId];
	//		}
	//		else
	//		{
	//			std::cout << "[WARNING]:Entity Id or Component not found \n";
	//			return nullptr;
	//		}
	//	}
	//	return nullptr;
	//}

	////Camera Logic
	//bool AttachToCamera(const std::vector<std::shared_ptr<Actor>>& _entities, const int& id)
	//{
	//	if (_entities.empty()) return false;

	//	for (const std::shared_ptr<Actor>& entity : _entities)
	//	{
	//		if (m_actorManager->HasComponent(id)) 
	//		{
	//			return m_actorManager->GetComponent(entity->mId)->actors[entity->mId]->mAttachToActor = true; 
	//		}
	//		else
	//		{
	//			std::cout << "[WARNING]:Entity Id or Component not found \n";
	//			return false;
	//		}
	//	}
	//	return false;
	//}



	////Redner Logic
	//void DrawEntity(const std::vector<std::shared_ptr<Actor>>& _entities, float dt) const
	//{
	//	if (_entities.empty()) return;
	//	for (const std::shared_ptr<Actor>& entity : _entities)
	//	{
	//		for (const auto& [id, component] : m_actorManager->GetAllComponents())
	//		{
	//			int counter = 0;
	//			for (const auto& it : component)
	//			{
	//				counter++;
	//				it->actors[id]->UpdateActors(dt);

	//			}
	//		}
	//	}
	//	return;
	//}
};