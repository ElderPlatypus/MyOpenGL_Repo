#include "Scene.h"

Scene::Scene(std::string name)
{
	mName = name;
}

///Loaders & Unloaders
void Scene::LoadActors()
{
	///Components
	numEntities = 1;
	for (int i = 0; i < numEntities; i++)
	{
		std::shared_ptr<Entity> entity = std::make_shared<Entity>(); 
		entity->SetId(i);

		mEntities.emplace_back(entity);

		std::shared_ptr<TransformComponent> transformComponent_1 = std::make_shared<TransformComponent>(); 
		transformComponent_1->m_pos.emplace_back(glm::vec3(5, 3, 1 ));
		transformComponent_1->_Size = 2;
		   
		std::shared_ptr<HealthComponent> healthComponent_1 = std::make_shared<HealthComponent>();
		healthComponent_1->health.emplace_back(100);

		std::shared_ptr<PlayerComponent> playerComponent = std::make_shared<PlayerComponent>();
		playerComponent->actors.emplace_back(std::make_shared<Actor>(Mesh::CreateCube(2.0f), "Gætt"));

		std::shared_ptr<ActorComponent> actorComponent = std::make_shared<ActorComponent>();
		actorComponent->spawn.emplace_back(Actor::spawnVector);    

		std::shared_ptr<DamageComponent> damageComponent = std::make_shared<DamageComponent>();
		damageComponent->damage.emplace_back(20);

		damageManager->AddComponent(entity->GetId(), damageComponent); 
		playerManager->AddComponent(entity->GetId(), playerComponent);  
		transformManager->AddComponent(entity->GetId(),transformComponent_1); 
		healthManager->AddComponent(entity->GetId(), healthComponent_1);
	}

	playerSystem = std::make_shared<PlayerSystem>(playerManager);
	playerSystem->Update(mEntities);
	playerSystem->AttachToCamera(mEntities, 0);
	playerSystem->UseTexture(mEntities,0); 
	playerSystem->GetPlayer(mEntities)->mEnableAABBCollision = true;

	damageSystem = std::make_shared<DamageSystem>(damageManager);
	damageSystem->Update(mEntities);






	Actor::Spawner(10, -20, 20, 2);
    for (const auto& actors : Actor::spawnVector)
	{
		actors->mEnableAABBCollision = true;
	}



	/////Player
	//uActorMap["player"] = std::make_shared<Actor>(Mesh::CreateCube(2.0f),"player"); 
	//uActorMap["player"]->SetTexBool(true);
	//uActorMap["player"]->EnablePhysics = false;
	//uActorMap["player"]->ExtrudeMesh(decrease, 2.0f); 
	//uActorMap["player"]->SetLocalPosition(glm::vec3(0, 0, 5));
	//uActorMap["player"]->mEnableAABBCollision = true; 
	//uActorMap["player"]->isPlayer = false;
	//uActorMap["player"]->mAttachToActor = false; 
	//uActorMap["player"]->mCanMove = true; 
	
	// Define knot vectors
	std::vector<float> uKnot = CreateRandomKnotVector<float>(7, 0.f, 2.f); 
	std::vector<float> vKnot = CreateRandomKnotVector<float>(7, 0.f, 2.f); 

	// Define control points (2D grid)
	std::vector<std::vector<glm::vec3>> controlPoints = CreateKnotVectorTuple(3,3, 0.f, 2.f); 
	
	uActorMap["surface"] = std::make_shared<Actor>(Mesh::CreateSplineSurface(20,20,2,2,uKnot,vKnot,controlPoints,10.f),"splineStuff");  
	uActorMap["surface"]->UseTexBool(true); 
	uActorMap["surface"]->SetLocalPosition(glm::vec3(0, 0, 0));
	
	///Create camera object
    mSceneCamera = std::make_shared<Camera>("SceneCamera");
	mSceneCamera->SetLocalPosition(glm::vec3(0, -10.0f, 10.f));

	

}

void Scene::LoadContent()
{
	LoadActors();

	mShader = std::make_shared<Shader>("Shaders/Triangle.vs", "Shaders/Triangle.fs");
	mTexture = std::make_shared<Texture>("Shaders/wall.jpg",mShader);     
	
	for (const std::pair<std::string,std::shared_ptr<Actor>> &actor : uActorMap) { actor.second->SetShader(mShader); }
	for (const auto& actor : Actor::spawnVector) { actor->SetShader(mShader); }
	playerSystem->SetShader(mEntities,mShader);
	
}

void Scene::UnloadContent()
{
	for (const std::pair<std::string, std::shared_ptr<Actor>> actor : uActorMap)
	{
		if (actor.second != nullptr)
		{
			actor.second->~Actor();
		}
	}

	playerSystem->GetPlayer(mEntities)->~Actor();

	mSceneCamera = nullptr;

	mShader = nullptr;

	mTexture->~Texture(); 

	mTexture = nullptr; 
	
	uActorMap.clear();
}

///Updater
void Scene::UpdateScene(float dt)
{
	//Camera Update
	mSceneCamera->UpdateCamera(dt); 
	BindCamera();

	//Actor Update for Bary coords
	
	if (!uActorMap.empty())
	{
		for (const std::pair<std::string, std::shared_ptr<Actor>> actor : uActorMap) { actor.second->UpdateActors(dt); }

	}
	if (!Actor::spawnVector.empty())
	{
	  for (const auto& actor : Actor::spawnVector) { actor->UpdateActors(dt); }
	}

	playerSystem->DrawEntity(mEntities, dt);
	for (const auto& object : Actor::projectileVector)
	{
		object->SetLocalRotation(playerSystem->GetPlayer(mEntities)->GetLocalRotation());
		object->SetLocalPosition(object->GetLocalPosition() + (playerSystem->GetPlayer(mEntities)->GetLocalPosition() * dt * 100.f));

		object->SetShader(mShader);
		object->UpdateActors(dt);
	}

	//Collison Update
	CollisionHandling(dt);
	SpaceManipulation();
	
}

///Rednerer
void Scene::RenderScene(float dt, Transform globaltransform)
{ 
	UpdateScene(dt);  
}

///Tranformations
void Scene::SpaceManipulation() const //Only rotation can be manipulated before call in Render. Offset needs to be set in LoacActors.
{
	///Spawn objects
	/*for (auto &object : Actor::spawnVector)
	{
		 object->mMesh->SetLocalRotation(glm::vec3((float)glfwGetTime(), (float)glfwGetTime(), (float)glfwGetTime())); 
	}*/

} 

///Shader Binder
void Scene::BindCamera() const
{
	mShader->setMat4("view", mSceneCamera->GetViewMatrix()); 
	mShader->setMat4("projection", mSceneCamera->GetProjectionMatrix());
	mShader->setVec3("viewPos", mSceneCamera->GetLocalPosition());   
}

void Scene::CollisionHandling(float dt) const
{
	//Player & spawned objects
	auto max_i = Actor::spawnVector.size();
	
	if (!Actor::spawnVector.empty())
	{
		for (const auto& object : Actor::spawnVector)
		{
			if (playerSystem->GetPlayer(mEntities)->isPlayer)
			{
				Collision::TrackPlayer(playerSystem->GetPlayer(mEntities), object, dt, 5);
			}
		}
	}
}


