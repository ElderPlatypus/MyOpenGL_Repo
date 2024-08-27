#include "Scene.h"
#include <random>


Scene::Scene(std::string name)
{
	mName = name;
}

///Loaders & Unloaders
void Scene::LoadActors()
{
	///Plane
	uActorMap["planeXZ"] = Actor::CreatePlaneXZ(-5.f, -5.f, 5.f, 5.f, 0.2f);  
	uActorMap["planeXZ"]->mUseTex = true; 
	uActorMap["planeXZ"]->SetLocalPosition(glm::vec3(-2.f, -1.0f, -8.f));

	///Player
	uActorMap["player"] = Actor::CreateCube();
	uActorMap["player"]->mEnableCollison = true;
	uActorMap["player"]->SetSurfaceActor(uActorMap["planeXZ"]);
	uActorMap["player"]->SetLocalPosition(glm::vec3(-2.f, 0.0f, -8.f));  
	uActorMap["player"]->mRelativeCameraPosition = true; 

	///Test cube
	uActorMap["testCube"] = Actor::CreateCube();
	uActorMap["testCube"]->mEnableCollison = true;
	uActorMap["testCube"]->SetSurfaceActor(uActorMap["planeXZ"]);
	uActorMap["testCube"]->SetLocalPosition(glm::vec3(-3.f, 0.0f, -6.f)); 

	///creating interpolation curve
	uActorMap["Icurve"] = Actor::CreateInterpolationCurve3Points(glm::vec2(-5.f, -8.f), glm::vec2(-3.f, -2.f), glm::vec2(-1.f, -2.f), -5, 5, 0.2f);
	uActorMap["Icurve"]->SetSurfaceActor(uActorMap["planeXZ"]);

	///Create camera object
    mSceneCamera = new Camera("SceneCamera"); 

	///Spawning vectors
	Actor::Spawner(5);
	for (auto &it : Actor::spawnVector)
	{
		it->SetSurfaceActor(uActorMap["planeXZ"]);
	}

}

void Scene::LoadContent()
{
	LoadActors();

	mShader = new Shader("Shaders/Triangle.vs", "Shaders/Triangle.fs");
	mTexture = new Texture("Shaders/wall.jpg",mShader);   
	
	for (auto actor = uActorMap.begin(); actor != uActorMap.end(); actor++) { actor->second->SetShader(mShader); }
	for (auto object : Actor::spawnVector){ object->SetShader(mShader); }         

}

void Scene::UnloadContent()
{
	for (auto actor = uActorMap.begin(); actor!= uActorMap.end(); actor++)
	{
		if (actor->second != nullptr)
		{
			actor->second->~Actor();
			delete actor->second;
			actor->second = nullptr;
		}
	
	}

	for (auto it : Actor::spawnVector)
	{
		it->~Actor();
		delete it;
		it = nullptr;
	}

	delete mSceneCamera;
	mSceneCamera = nullptr;

	delete mShader;
	mShader = nullptr;

	mTexture->~Texture(); 
	delete mTexture; 
	mTexture = nullptr; 
}

///Updater
void Scene::UpdateScene(float dt)
{
	//Camera Update
	mSceneCamera->UpdateCamera(dt); 

	//Actor Update for Bary coords
	for (auto actor = uActorMap.begin(); actor != uActorMap.end(); actor++) { actor->second->UpdateActors(dt); }
	for (auto object : Actor::spawnVector) { object->UpdateActors(dt); }

	//Collison Update
	CollisionHandling(dt);

}

///Rednerer
void Scene::RenderScene(float dt, Transform globaltransform)
{ 
	SpaceManipulation(); 
	BindCamera();
	UpdateScene(dt);  

	for (auto actor = uActorMap.begin(); actor!= uActorMap.end(); actor++) 
	{
		globaltransform.SetTransformMatrix(actor->second->GetLocalTransformMatrix());
		mShader->setMat4("model", globaltransform.GetTransformMatrix());   
		actor->second->UseTexture(actor->second->GetTexBool());    
		actor->second->drawActor(mShader); 
	}
	
	for (auto object : Actor::spawnVector)
	{
		globaltransform.SetTransformMatrix(object->GetLocalTransformMatrix());
		mShader->setMat4("model", object->GetLocalTransformMatrix());
		object->UseTexture(object->GetTexBool());
		object->drawActor(mShader);
	}
}

///Tranformations
void Scene::SpaceManipulation() //Only rotation can be manipulated before call in Render. Offset needs to be set in LoacActors.
{
	///Spawn objects

	for (auto object : Actor::spawnVector)
	{
		 object->SetLocalRotation(glm::vec3((float)glfwGetTime(), (float)glfwGetTime(), (float)glfwGetTime())); 
	}

} 

///Shader Binder
void Scene::BindCamera() const
{
	mShader->setMat4("view", mSceneCamera->GetViewMatrix()); 
	mShader->setMat4("projection", mSceneCamera->GetProjectionMatrix());
	mShader->setVec3("viewPos", mSceneCamera->GetLocalPosition());   
}

void Scene::CollisionHandling(float dt)
{
	//Player & spawned objects
	int max_i = Actor::spawnVector.size();
	
	
	for (auto object : Actor::spawnVector) 
	{ 
		if (Collision::Intersect(uActorMap["player"], object) == true) 
		{
			std::cout << "Spawned Cube Deleted" << std::endl;

			object->~Actor();
			object->mEnableCollison = false;
			score++;
		    std::cout << "Points earned: " << score << " off " << max_i << std:: endl;

			if (score == 10)
			{
				std::cout << "Maximum points earned!!!: " << std::endl;
			}
		}
	}
	
	//Player & TestCube
	Collision::Intersect(uActorMap["player"], uActorMap["testCube"]);
	if (Collision::Intersect(uActorMap["player"], uActorMap["testCube"]) == true)
	{ 
		std::cout << "Test Cube Deleted" << std::endl;
		uActorMap["testCube"]->~Actor();
		uActorMap["testCube"]->mEnableCollison = false;
	}
}


