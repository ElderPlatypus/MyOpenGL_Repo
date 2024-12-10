#include "Scene.h"

Scene::Scene(std::string name)
{
	mName = name;
}

///Loaders & Unloaders
void Scene::LoadActors()
{
	///Spawner_____________________________________________________
	uSpawnerMap["Balls"] = std::make_shared<Spawner>(100,60.f,60.f,MeshShape::Sphere);
	uSpawnerMap["Balls"]->UseTexture_all(TextureType::Wall);
	uSpawnerMap["Balls"]->EnablePhysics_all(true);
	uSpawnerMap["Balls"]->SetPosition_all(glm::vec3(0.f, -150.f, 0.f));


	///Regular Triangulation Plane_____________________________________________________
	//uActorMap["RegularSurfaceXZ"] = std::make_shared<Actor>(Mesh::CreatePlaneXZ(-20.f, -20.f, 20.f, 20.f, 0.1f),"baryMesh");
	//uActorMap["RegularSurfaceXZ"]->UseTexConfig(true, TextureType::Container); 
	//uActorMap["RegularSurfaceXZ"]->UseLightConfig(true,LightType::Slope);
	//uActorMap["RegularSurfaceXZ"]->SetLocalPosition(glm::vec3(0, 10.f, 0)); 

	//uActorMap["Ground"] = std::make_shared<Actor>(Mesh::CreateFlastSurfaceXZ(10.f), "groundMesh");
	//uActorMap["Ground"]->UseTexConfig(true, TextureType::Container); 
	//uActorMap["Ground"]->SetLocalPosition(glm::vec3(0, 10.f, 0));


	///Player_____________________________________________________
	uActorMap["Player"] = std::make_shared<Actor>(Mesh::CreateSphere(30,30,40), "PlayerMesh", CollisionType::BallBall);    
	uActorMap["Player"]->UseTexConfig(true, TextureType::SnowFlake);
	uActorMap["Player"]->UseLightConfig(true,LightType::Default); 
	uActorMap["Player"]->isPlayer = true;
	uActorMap["Player"]->mAttachToActor = true;
	uActorMap["Player"]->SetLocalPosition(glm::vec3(0.f, 0.f, -20.f));
	//uActorMap["Player"]->AddComponentArchive("Damage", damageManager); 

	///Test Sphere
	uActorMap["Test"] = std::make_shared<Actor>(Mesh::CreateSphere(20, 20, 8), "TestMesh", CollisionType::BallBall);  
	uActorMap["Test"]->UseTexConfig(true, TextureType::Wall); 
	uActorMap["Test"]->UseLightConfig(true, LightType::Default);
	uActorMap["Test"]->isPlayer = true;
	uActorMap["Test"]->mAttachToActor = true;
	uActorMap["Test"]->SetLocalPosition(glm::vec3(0.f, -100.f, 0.f));
	uActorMap["Test"]->EnablePhysics(true);


	///Components_____________________________________________________
	std::shared_ptr<HealthComponent> healthComponent_1 = std::make_shared<HealthComponent>();
	healthComponent_1->health.emplace_back(100);

	std::shared_ptr<DamageComponent> damageComponent = std::make_shared<DamageComponent>();
	damageComponent->damage.emplace_back(20);

	std::shared_ptr<PhysicsComponent> physicsComponent_1 = std::make_shared<PhysicsComponent>();
	physicsComponent_1->m_EnablePhysics.emplace_back(true);

	/*std::shared_ptr<SceneGraphComponent> sceneGraphComponent = std::shared_ptr<SceneGraphComponent>();
	sceneGraphComponent->uActorMap["cCube"] = std::make_shared<Actor>(Mesh::CreateCube(2), "cCube", CollisionType::AABB); 

	healthManager->AddComponent("Health", healthComponent_1);
	damageManager->AddComponent("Damage", damageComponent);
	physicsManager->AddComponent("Physic", physicsComponent_1);
	sceneGraphManager->AddComponent("SceneGraph", sceneGraphComponent);*/


	
	///Camera_____________________________________________________
    mSceneCamera = std::make_shared<Camera>("SceneCamera"); 
	mSceneCamera->SetLocalPosition(glm::vec3(0, -10.0f, 150.f));
	mSceneCamera->SetAccelerationSpeed(100.f);
	mSceneCamera->mFarplane = 800.f; 
	mSceneCamera->UpdateProjectionMatrix();

	///LAS_____________________________________________________
	auto folder1 = LoadVectorOfPath("../myopengl/mathlib/terraindata/t1/data/", "c:/users/lenovo/documents/spilltech2024_h/myopengl_repo/myopengl_main/myopengl/mathlib/terraindata/t1/data");
	auto folder2 = LoadVectorOfPath("../myopengl/mathlib/terraindata/t2/data/", "c:/users/lenovo/documents/spilltech2024_h/myopengl_repo/myopengl_main/myopengl/mathlib/terraindata/t2/data");
	auto folder3 = LoadVectorOfPath("../myopengl/mathlib/terraindata/t3/data/","c:/users/lenovo/documents/spilltech2024_h/myopengl_repo/myopengl_main/myopengl/mathlib/terraindata/t3/data");
	auto folder4 = LoadVectorOfPath("../myopengl/mathlib/terraindata/t4/data/", "c:/users/lenovo/documents/spilltech2024_h/myopengl_repo/myopengl_main/myopengl/mathlib/terraindata/t4/data");
	
	//uActorMap["grid"] = std::make_shared<Actor>(Mesh::CreateGridFromLas(folder4[1].c_str(), 0.2f, 300), "TriangulationGrid");  ;      
	//uActorMap["grid"]->UseLightConfig(true, LightType::Slope); 
	//uActorMap["grid"]->SetLocalPosition(glm::vec3(0.f, -200.f, 0.f));

	///Box limit
	uBoundingMap["Box"] = std::make_shared<Actor>(Mesh::CreateCube(300), "Box", CollisionType::AABBInverse); 
	uBoundingMap["Box"]->UseTexConfig(true, TextureType::Container);


	///ParticleSystems
	//uParticleMap["snow"] = std::make_shared<ParticleSimulation>(1000, MeshShape::Sphere, ParticleType::Snow);     
	//uParticleMap["snow"]->SnowParticle(0.f, 100.f, uBoundingMap["Box"],uActorMap);

	uParticleMap["water"] = std::make_shared<ParticleSimulation>(1000, MeshShape::Sphere, ParticleType::Rain);
	uParticleMap["water"]->RainParticle(0.f, 100.f, uBoundingMap["Box"], uActorMap); 
}

void Scene::LoadContent()
{
	///Actors_____________________________________________________
	LoadActors();

	///Shaders_____________________________________________________
	mShader = std::make_shared<Shader>("Shaders/VertexShader.vs", "Shaders/FragmentShader.fs");

	uTextureMap["wall"] = std::make_shared<Texture>(0, GL_TEXTURE0, "texture1", "Shaders/wall.jpg", mShader);
	uTextureMap["container"] = std::make_shared<Texture>(1, GL_TEXTURE1, "texture2", "Shaders/container.jpg", mShader);
	uTextureMap["snowflake"] = std::make_shared<Texture>(2, GL_TEXTURE2,"texture3", "Shaders/snowflake.jpg", mShader);

	///Apply Shaders_____________________________________________________
	Set_uMapShader(mShader);
}

void Scene::UnloadContent()
{
	///Unload Content_____________________________________________________
	if (!uActorMap.empty())
	{
		for (const std::pair<std::string, std::shared_ptr<Actor>>& actor : uActorMap) { actor.second->~Actor(); }
	}
	if (!uParticleMap.empty())
	{
		for (const std::pair<std::string, std::shared_ptr<ParticleSimulation>>& actor : uParticleMap) { actor.second->~ParticleSimulation(); }
	}
	if (!uSpawnerMap.empty()) 
	{
		for (const std::pair<std::string, std::shared_ptr<Spawner>>& actor : uSpawnerMap) { actor.second->~Spawner(); }
	}
	if (!uTextureMap.empty())
	{
		for (const std::pair<std::string, std::shared_ptr<Texture>>& actor : uTextureMap) { actor.second->~Texture(); }
	}
	for (const auto& folder : terrainData)
	{
		folder.~vector();
	
	}

	mSceneCamera = nullptr;
	mShader = nullptr;
	uActorMap.clear();
	uTextureMap.clear();
	uSpawnerMap.clear();
}

void Scene::UpdateScene(float dt)
{
	///Camera Update_____________________________________________________
	mSceneCamera->UpdateCamera(dt);   
	BindCamera();
	///Actor Update_____________________________________________________
	Update_uMapActors(dt);
	///Camera Update_____________________________________________________
	CollisionHandling(dt);
	SpaceManipulation();
	
}

void Scene::Update_uMapActors(float dt)
{ 
	if (!uActorMap.empty()) 
	{
		for (const std::pair<std::string, std::shared_ptr<Actor>>& actor : uActorMap) { actor.second->UpdateActors(dt); }
	}
	if (!uBoundingMap.empty()) 
	{
		for (const std::pair<std::string, std::shared_ptr<Actor>>& actor : uBoundingMap) { actor.second->UpdateActors(dt); }
	}
	if (!uParticleMap.empty())
	{
		for (const std::pair<std::string, std::shared_ptr<ParticleSimulation>>& actor : uParticleMap) { actor.second->Update(dt); }
	}
	if (!uSpawnerMap.empty())
	{
		for (const std::pair<std::string, std::shared_ptr<Spawner>>& actor : uSpawnerMap) { actor.second->UpdateAll(dt);}
	}
}

void Scene::Set_uMapShader(const std::shared_ptr<Shader>& shader)
{
	if (!uActorMap.empty())
	{
		for (const std::pair<std::string, std::shared_ptr<Actor>>& actor : uActorMap) { actor.second->SetShader(shader); }
	}
	if (!uBoundingMap.empty())
	{
		for (const std::pair<std::string, std::shared_ptr<Actor>>& actor : uBoundingMap) { actor.second->SetShader(shader); }
	}
	if (!uParticleMap.empty())
	{
		for (const std::pair<std::string, std::shared_ptr<ParticleSimulation>>& actor : uParticleMap) { actor.second->SetShader(shader); }
	}
	if (!uSpawnerMap.empty()) 
	{
		for (const std::pair<std::string, std::shared_ptr<Spawner>>& actor : uSpawnerMap) { actor.second->SetShaderAll(shader); } 
	}
}

void Scene::RenderScene(float dt, Transform globaltransform)
{ 
	///Render Scene_____________________________________________________
	UpdateScene(dt);  
}

void Scene::SpaceManipulation() const //Only rotation can be manipulated before call in Render. Offset needs to be set in LoacActors.
{
	///Spawn objects
	/*for (auto &object : Actor::spawnVector)
	{
		 object->mMesh->SetLocalRotation(glm::vec3((float)glfwGetTime(), (float)glfwGetTime(), (float)glfwGetTime())); 
	}*/

	//uActorMap.find("Player")->second->SetLocalRotation(glm::vec3((float)glfwGetTime(), (float)glfwGetTime(), (float)glfwGetTime()));
} 

void Scene::BindCamera() const
{
	mShader->setMat4("view", mSceneCamera->GetViewMatrix()); 
	mShader->setMat4("projection", mSceneCamera->GetProjectionMatrix());
	mShader->setVec3("viewPos", mSceneCamera->GetLocalPosition());   
}

void Scene::CollisionHandling(float dt) 
{
	//Player & spawned objects
	auto getFric = MathLib::CalculateFriction(uBoundingMap["Box"]); 

	if (!uSpawnerMap["Balls"]->spawnVector.empty())
	{
		for (size_t i = 0; i < uSpawnerMap["Balls"]->spawnVector.size(); i++)
		{
			//Current actor
			auto& actor = uSpawnerMap["Balls"]->spawnVector[i];

			//CollisionCheck
			bool isColliding = Collision::AABBInverse<Actor, Actor>(actor, uBoundingMap["Box"]);


			if (isColliding)  
			{
				//Set friction
				actor->rigidB->Roll(glm::vec3(0.0f,-1.0f,0.0f));
				actor->rigidB->ApplyFriction(getFric);  



				//Check for collison with other actors
				for (size_t j = i + 1; j < uSpawnerMap["Balls"]->spawnVector.size(); j++)
			    {
				    auto& otherActor = uSpawnerMap["Balls"]->spawnVector[j];
				     
					Collision::BallBall<Actor, Actor>(actor, otherActor);
			    }
			}
		}
	}
	if (Collision::AABBInverse(uActorMap["Test"], uBoundingMap["Box"]))
	{
		uActorMap["Test"]->rigidB->Roll(glm::vec3(0.0f, -1.0f, 0.0f));
	}
	
	Collision::BallBall(uActorMap["Test"], uActorMap["Player"]);
}


