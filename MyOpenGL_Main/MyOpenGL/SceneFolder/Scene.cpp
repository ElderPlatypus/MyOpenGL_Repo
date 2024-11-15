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
	

		std::shared_ptr<TransformComponent> transformComponent_1 = std::make_shared<TransformComponent>(); 
		transformComponent_1->m_pos.emplace_back(glm::vec3(5, 3, 1 ));
		transformComponent_1->_Size = 2;
		   
		std::shared_ptr<HealthComponent> healthComponent_1 = std::make_shared<HealthComponent>();
		healthComponent_1->health.emplace_back(100);

		std::shared_ptr<ActorComponent> actorComponent = std::make_shared<ActorComponent>();
		actorComponent->spawn.emplace_back(Actor::spawnVector);    

		std::shared_ptr<DamageComponent> damageComponent = std::make_shared<DamageComponent>();
		damageComponent->damage.emplace_back(20);

		/*damageManager->AddComponent(entity->GetId(), damageComponent); 
		transformManager->AddComponent(entity->GetId(),transformComponent_1); 
		healthManager->AddComponent(entity->GetId(), healthComponent_1);*/
	}

	Actor::Spawner(10, -50, 50, 2);
    for (const auto& actors : Actor::spawnVector)
	{
		actors->mEnableAABBCollision = true;
	}
	uActorMap["Player"] = std::make_shared<Actor>(Mesh::CreateCube(2.0f), "PlayerMesh");
	uActorMap["Player"]->UseTexBool(true);
	uActorMap["Player"]->isPlayer = true;
	uActorMap["Player"]->mAttachToActor = true;


	
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
	//mSceneCamera->SetAccelerationSpeed(10.f);
	mSceneCamera->mFarplane = 500.f; 
	mSceneCamera->UpdateProjectionMatrix();

	int counter = 0;
	int counter2 = 0;
	
	//auto folder1 = LoadVectorOfPath("../myopengl/mathlib/terraindata/t1/data/", "c:/users/lenovo/documents/spilltech2024_h/myopengl_repo/myopengl_main/myopengl/mathlib/terraindata/t1/data");
	//auto folder2 = LoadVectorOfPath("../myopengl/mathlib/terraindata/t2/data/", "c:/users/lenovo/documents/spilltech2024_h/myopengl_repo/myopengl_main/myopengl/mathlib/terraindata/t2/data");
	auto folder3 = LoadVectorOfPath("../myopengl/mathlib/terraindata/t3/data/","c:/users/lenovo/documents/spilltech2024_h/myopengl_repo/myopengl_main/myopengl/mathlib/terraindata/t3/data");
	//auto folder4 = LoadVectorOfPath("../myopengl/mathlib/terraindata/t4/data/", "c:/users/lenovo/documents/spilltech2024_h/myopengl_repo/myopengl_main/myopengl/mathlib/terraindata/t4/data");
	
	 
	//terrainData.emplace_back(folder1);   
	//terrainData.emplace_back(folder2);   
	terrainData.emplace_back(folder3);   
	//terrainData.emplace_back(folder4);  

	for (const auto& files : terrainData)
	{
		//std::cout << "[LOG]:Reading from folder:" << counter2 << "\n";
		for (int i = 0; i < files.size(); i++)
		{
			counter++;
			//std::cout << "[LOG]:Path is:" << content.c_str() << "\n";
			uActorMap["terrain" + std::to_string(counter)] = std::make_shared<Actor>(Mesh::CreatePointCloudFromLASFileSurface(files[0].c_str(), 0.02f), "Terrain");
			//uActorMap["terrain" + std::to_string(counter)]->SetLocalPosition(glm::vec3(counter2 * 100.f, 0, 0));  
		}
		counter2++;
		std::cout << "\n";
	}
}

void Scene::LoadContent()
{
	LoadActors();

	mShader = std::make_shared<Shader>("Shaders/Triangle.vs", "Shaders/Triangle.fs");
	mTexture = std::make_shared<Texture>("Shaders/wall.jpg",mShader);     
	
	for (const std::pair<std::string,std::shared_ptr<Actor>> &actor : uActorMap) { actor.second->SetShader(mShader); }
	for (const auto& actor : Actor::spawnVector) { actor->SetShader(mShader); }
	
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

	for (const auto& folder : terrainData)
	{
		folder.~vector();
	
	}
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


	//Collison Update
	CollisionHandling(dt);
	SpaceManipulation();
	
	Actor::ProjectileSpawner(uActorMap.find("Player")->second, mShader, dt); 
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

	//uActorMap.find("Player")->second->SetLocalRotation(glm::vec3((float)glfwGetTime(), (float)glfwGetTime(), (float)glfwGetTime()));

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
			if (uActorMap.find("Player")->second->isPlayer) 
			{ 
				//Collision::TrackPlayer(uActorMap.find("Player")->second, object, dt, 5);
			}
		}
	}
	if (!Actor::projectileVector.empty())
	{
		for (const auto& projectile : Actor::projectileVector)
		{
			if (!Actor::spawnVector.empty())
			{
				for (const auto& object : Actor::spawnVector)
				{
					Collision::ProjectileHit(projectile, object);
				}
			}
			
		}
	}
}


