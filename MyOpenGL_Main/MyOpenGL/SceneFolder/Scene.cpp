#include "Scene.h"

Scene::Scene(std::string name)
{
	mName = name;
}

///Loaders & Unloaders
void Scene::LoadActors()
{
	///Spawner_____________________________________________________
	Actor::Spawner(10, -50, 50, 2);
    for (const auto& actors : Actor::spawnVector)
	{
		actors->mEnableAABBCollision = true;
	}
	///Regular Triangulation Plane_____________________________________________________
	/*uActorMap["RegularSurfaceXZ"] = std::make_shared<Actor>(Mesh::CreatePlaneXZ(-20.f, -20.f, 20.f, 20.f, 0.1f),"baryMesh");
	uActorMap["RegularSurfaceXZ"]->UseTexConfig(true, 0); 
	uActorMap["RegularSurfaceXZ"]->UseLightConfig(true,1);
	uActorMap["RegularSurfaceXZ"]->SetLocalPosition(glm::vec3(0, 10.f, 0)); */


	///Player_____________________________________________________
	//uActorMap["Player"] = std::make_shared<Actor>(Mesh::CreateCircle(glm::vec3(0.f, 0.f, 0.f), 5.f, 20), "PlayerMesh");
	uActorMap["Player"] = std::make_shared<Actor>(Mesh::CreateCube(2.f), "PlayerMesh");
	uActorMap["Player"]->UseTexConfig(true,1);
	uActorMap["Player"]->UseLightConfig(false,1);
	uActorMap["Player"]->isPlayer = true;
	uActorMap["Player"]->mAttachToActor = true;
	//uActorMap["Player"]->SetBarySurfaceMesh(uActorMap["RegularSurfaceXZ"]->getMesh());   
	uActorMap["Player"]->AddComponentArchive("Damage", damageManager);

	///Components_____________________________________________________
	std::shared_ptr<HealthComponent> healthComponent_1 = std::make_shared<HealthComponent>();
	healthComponent_1->health.emplace_back(100);

	std::shared_ptr<DamageComponent> damageComponent = std::make_shared<DamageComponent>();
	damageComponent->damage.emplace_back(20);

	healthManager->AddComponent("Health", healthComponent_1);
	damageManager->AddComponent("Damage", damageComponent);


	///B_Spline_____________________________________________________
	// Define knot vectors
	std::vector<float> uKnot = MathLib::CreateRandomKnotVector<float>(7, 0.f, 2.f);  
	std::vector<float> vKnot = MathLib::CreateRandomKnotVector<float>(7, 0.f, 2.f);

	// Define control points (2D grid)
	std::vector<std::vector<glm::vec3>> controlPoints = MathLib::CreateKnotVectorTuple(3,3, 0.f, 2.f);
	
	/*uActorMap["Bspline"] = std::make_shared<Actor>(Mesh::CreateSplineSurface(20,20,2,2,uKnot,vKnot,controlPoints,10.f),"splineStuff");  
	uActorMap["Bspline"]->UseTexBool(true); 
	uActorMap["Bspline"]->SetLocalPosition(glm::vec3(0, 0, 0));*/


	
	///Camera_____________________________________________________
    mSceneCamera = std::make_shared<Camera>("SceneCamera");
	mSceneCamera->SetLocalPosition(glm::vec3(0, -10.0f, 10.f));
	//mSceneCamera->SetAccelerationSpeed(10.f);
	mSceneCamera->mFarplane = 800.f; 
	mSceneCamera->UpdateProjectionMatrix();

	///LAS_____________________________________________________
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
		for (int i = 0; i < files.size(); i++)
		{
			counter++;
			uActorMap["grid"] = std::make_shared<Actor>(Mesh::CreateGridFromLas(files[0].c_str(), 0.2f,250), "TriangulationGrid");
			uActorMap["grid"]->UseTexConfig(true, 0);   
			uActorMap["grid"]->UseLightConfig(true,1);    
			uActorMap["grid"]->SetLocalPosition(glm::vec3(0.f, 0.f, 0.f));
	        //uActorMap["Player"]->SetBarySurfaceMesh(uActorMap["grid"]->getMesh()); 
			//uActorMap["pointcloud"] = std::make_shared<Actor>(Mesh::CreatePointCloudFromLASFileSurface(files[0].c_str(), 0.02f), "Terrain");
			//uActorMap["pointcloud"]->UseLightConfig(false);
			//uActorMap["pointcloud"]->SetLocalPosition(glm::vec3(0.f, 50.f, 0.f));
			return;
		}
	}

}

void Scene::LoadContent()
{
	///Actors_____________________________________________________
	LoadActors();

	///Shaders_____________________________________________________
	mShader = std::make_shared<Shader>("Shaders/VertexShader.vs", "Shaders/FragmentShader.fs");

	uTextureMap["wall"] = std::make_shared<Texture>(0, GL_TEXTURE0, "texture1", "Shaders/wall.jpg", mShader);
	uTextureMap["container"] = std::make_shared<Texture>(1,GL_TEXTURE1,"texture2", "Shaders/container.jpg", mShader);  

	///Apply Shaders_____________________________________________________
	for (const std::pair<std::string,std::shared_ptr<Actor>> &actor : uActorMap) { actor.second->SetShader(mShader); }
	for (const auto& actor : Actor::spawnVector) { actor->SetShader(mShader); }
	
}

void Scene::UnloadContent()
{
	///Unload Content_____________________________________________________
	for (const std::pair<std::string, std::shared_ptr<Actor>> actor : uActorMap)
	{
		if (actor.second)
		{
			actor.second->~Actor();
		}
	}

	for (const std::pair<std::string, std::shared_ptr<Texture>> texture : uTextureMap)
	{
		if (texture.second)
		{
			texture.second->~Texture();
		}
	}

	for (const auto& folder : terrainData)
	{
		folder.~vector();
	
	}
	mSceneCamera = nullptr;

	mShader = nullptr;

	
	uActorMap.clear();
	uTextureMap.clear();
}

void Scene::UpdateScene(float dt)
{
	///Camera Update_____________________________________________________
	mSceneCamera->UpdateCamera(dt); 
	BindCamera();

	///Actor Update_____________________________________________________
	if (!uActorMap.empty())
	{
		for (const std::pair<std::string, std::shared_ptr<Actor>> actor : uActorMap) { actor.second->UpdateActors(dt); }
	}
	///Spawner Update_____________________________________________________
	if (!Actor::spawnVector.empty())
	{
	  for (const auto& actor : Actor::spawnVector) { actor->UpdateActors(dt); }
	}
	Actor::ProjectileSpawner(uActorMap.find("Player")->second, mShader, dt); 


	///Camera Update_____________________________________________________
	CollisionHandling(dt);
	SpaceManipulation();
	
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
					Collision::ProjectileHit<Actor,Actor>(projectile, object);
				}
			}
			
		}
	}
}


