#include "Scene.h"

Scene::Scene(std::string name)
{
	mName = name;
}

///Loaders & Unloaders
void Scene::LoadActors()
{
	///Spawner_____________________________________________________
	Spawner::SpawnActors(5, -50, 50, MeshShape::Sphere);
	Spawner::UseTexture_all(TextureType::Wall); 
	Spawner::EnablePhysics_all(true);  
	Spawner::SetPosition_all(glm::vec3(0.f, 50.f, 0.f)); 

	///Regular Triangulation Plane_____________________________________________________
	//uActorMap["RegularSurfaceXZ"] = std::make_shared<Actor>(Mesh::CreatePlaneXZ(-20.f, -20.f, 20.f, 20.f, 0.1f),"baryMesh");
	//uActorMap["RegularSurfaceXZ"]->UseTexConfig(true, TextureType::Container); 
	//uActorMap["RegularSurfaceXZ"]->UseLightConfig(true,LightType::Slope);
	//uActorMap["RegularSurfaceXZ"]->SetLocalPosition(glm::vec3(0, 10.f, 0)); 


	///Player_____________________________________________________
	//uActorMap["Player"] = std::make_shared<Actor>(Mesh::CreateCircle(glm::vec3(0.f, 0.f, 0.f), 5.f, 20), "PlayerMesh");
	uActorMap["Player"] = std::make_shared<Actor>(Mesh::CreateCube(3.f), "PlayerMesh");
	uActorMap["Player"]->UseTexConfig(true, TextureType::Container);
	uActorMap["Player"]->UseLightConfig(true,LightType::Default); 
	uActorMap["Player"]->isPlayer = true;
	uActorMap["Player"]->mAttachToActor = true;
	//uActorMap["Player"]->SetBarySurfaceMesh(uActorMap["RegularSurfaceXZ"]->getMesh());   
	//uActorMap["Player"]->AddComponentArchive("Damage", damageManager);

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
	mSceneCamera->SetLocalPosition(glm::vec3(0, -100.0f, 300.f));
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
		for (int i = 0; i < 1; i++)
		{
			counter++;
			uActorMap["grid"] = std::make_shared<Actor>(Mesh::CreateGridFromLas(files[0].c_str(), 0.2f,300), "TriangulationGrid");
			//uActorMap["grid"]->UseTexConfig(true, 0);   
			uActorMap["grid"]->UseLightConfig(true,LightType::Slope);    
			//uActorMap["grid"]->SetLocalPosition(glm::vec3(0.f, -200.f, 0.f));

			//uActorMap["pointcloud"] = std::make_shared<Actor>(Mesh::CreatePointCloudFromLASFileSurface(files[0].c_str(), 0.02f), "Terrain");
			//uActorMap["pointcloud"]->UseLightConfig(false); 
			//uActorMap["pointcloud"]->SetLocalPosition(glm::vec3(0.f, 50.f, 0.f));
		}
	}

	uActorMap["Player"]->SetBarySurfaceMesh(uActorMap["grid"]->getMesh());
	//Spawner::SetBarySurfaceMesh_all(uActorMap["grid"]->getMesh());
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
	Set_uMapShader(mShader);
	Spawner::SetShaderSpawnVector(mShader); 
}

void Scene::UnloadContent()
{
	///Unload Content_____________________________________________________
	for (const std::pair<std::string, std::shared_ptr<Actor>>& actor : uActorMap)
	{
		if (actor.second)
		{
			actor.second->~Actor();
		}
	}

	for (const std::pair<std::string, std::shared_ptr<Texture>>& texture : uTextureMap)
	{
		if (texture.second)
		{
			texture.second->~Texture();
		}
	}

	for (const auto& actor : Spawner::spawnList)
	{
	  actor->~Actor();
	}

	for (const auto& folder : terrainData)
	{
		folder.~vector();
	
	}

	mSceneCamera = nullptr;
	mShader = nullptr;
	uActorMap.clear();
	uTextureMap.clear();
	Spawner::spawnList.clear();
}

void Scene::UpdateScene(float dt)
{
	uActorMap["Player"]->UpdateBarycentricCoords(dt); 
	///Camera Update_____________________________________________________
	mSceneCamera->UpdateCamera(dt);   
	BindCamera();
	///Actor Update_____________________________________________________
	Update_uMapActors(dt);
	///Spawner Update_____________________________________________________
	Spawner::UpdateSpawnVector(dt);
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
}

void Scene::Set_uMapShader(const std::shared_ptr<Shader>& shader)
{
	if (!uActorMap.empty())
	{
		for (const std::pair<std::string, std::shared_ptr<Actor>>& actor : uActorMap) { actor.second->SetShader(shader); }
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
	
	if (!Spawner::spawnList.empty()) 
	{
		for (const auto& actor : Spawner::spawnList)
		{
			/*if (actor->rigidB->GetLocalPosition().y < uActorMap["grid"]->GetLocalPosition().y) 
			{
				actor->rigidB->SetLocalPosition(glm::vec3(actor->rigidB->pos.x, uActorMap["grid"]->GetLocalPosition().y - actor->mExtent.y, actor->GetLocalPosition().x));
			}*/

			bool isColliding = Collision::AABB_y<Actor, Actor>(actor, uActorMap["grid"]);

			if (isColliding)  
			{
				Spawner::SetBarySurfaceMesh_all(uActorMap["grid"]->getMesh());

				actor->UpdateBarycentricCoordsRigidB(actor, dt);  
				//glm::vec3 baryCoords
				//actor->rigidB->Reflect(glm::vec3(0.0f, 0.1f, 0.0f)); 
				//actor->rigidB->RigidBodyStop(); 
				//actor->rigidB->Roll(actor->rigidB->GetLocalPosition());        
				//actor->rigidB->ConstraintPosition(uActorMap["grid"]->mCenter,uActorMap["grid"]->mExtent);  
				 
			}
			else
			{
				Spawner::ClearBarySurfaceMesh_all();
			}
		}
	}
}


