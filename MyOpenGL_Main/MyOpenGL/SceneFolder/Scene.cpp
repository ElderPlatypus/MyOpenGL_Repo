#include "Scene.h"
#include <random>


Scene::Scene(std::string name)
{
	mName = name;
}

///Loaders & Unloaders
void Scene::LoadActors()
{
	///Player
	uActorMap["player"] = new Actor(Mesh::CreateCube(2.0f),"player");
	uActorMap["player"]->mMesh->mUseTex = false;
	uActorMap["player"]->mRelativeCameraPosition = true; 
	//uActorMap["player"]->ExtrudeMesh(Extrude::increase, 1.0f);


	///Curve
	uActorMap["Icurve"] = new Actor(Mesh::CreatePlaneXZ(-5, -5, 5, 5, 0.5),"Icurve");
	/*uActorMap["Icurve"]->mMesh->mUseTex = true;  
	uActorMap["player"]->SetBarySurfaceMesh(uActorMap["Icurve"]->mMesh);  */


	///Test cube
	uActorMap["testCube"] = new Actor(Mesh::CreateCube(2.0f),"TestCube"); 
	uActorMap["testCube"]->mMesh->mDrawLine = true;
	uActorMap["testCube"]->ExtrudeMesh(Extrude::increase, 10.0f);
	

	
	///Spawning vectors
	Actor::ActorType = 1;
	Actor::Spawner(10,-20.f,20.f);

	for (int i = 0; i < Actor::spawnVector.size(); i++) 
	{
	  uActorMap["spawnedObjects" + std::to_string(i)] = Actor::spawnVector[i]; 
	}

	//for (auto& objects : uActorMap)
	//{
	//	if (objects.second->mName == "spawnedObjects")
	//	{
	//		objects.second->SetBarySurfaceMesh(uActorMap["Icurve"]->mMesh);
	//	}
	//}
	///Create camera object
    mSceneCamera = new Camera("SceneCamera"); 

}

void Scene::LoadContent()
{
	LoadActors();

	mShader = new Shader("Shaders/Triangle.vs", "Shaders/Triangle.fs");
	mTexture = new Texture("Shaders/wall.jpg",mShader);   
	
	for (auto &actor : uActorMap) { actor.second->mMesh->SetShader(mShader); }       
}

void Scene::UnloadContent()
{
	for (auto &actor : uActorMap)
	{
		if (actor.second != nullptr)
		{
			actor.second->~Actor();
			delete actor.second;
			actor.second = nullptr;
		}
	}

	delete mSceneCamera;
	mSceneCamera = nullptr;

	delete mShader;
	mShader = nullptr;

	mTexture->~Texture(); 
	delete mTexture; 
	mTexture = nullptr; 
	
	uActorMap.clear();
}

///Updater
void Scene::UpdateScene(float dt)
{
	//Camera Update
	mSceneCamera->UpdateCamera(dt); 
	/*uActorMap["player"]->cameraTracker(mSceneCamera, dt); */

	//Actor Update for Bary coords
	for (auto &actor : uActorMap) { actor.second->UpdateActors(dt); }
	for (auto &object : Actor::spawnVector) { object->UpdateActors(dt); }

	//Collison Update
	CollisionHandling(dt);

	SpaceManipulation();
	BindCamera();
}

///Rednerer
void Scene::RenderScene(float dt, Transform globaltransform)
{ 
	UpdateScene(dt);  

	for (auto &actor : uActorMap) 
	{
		globaltransform.SetTransformMatrix(actor.second->mMesh->GetLocalTransformMatrix());
		mShader->setMat4("model", globaltransform.GetTransformMatrix());   
		actor.second->mMesh->UseTexture(actor.second->mMesh->GetTexBool());    
		actor.second->mMesh->UseLight(actor.second->mMesh->GetLightBool());
		actor.second->mMesh->drawActor(mShader); 
	}
	
}

///Tranformations
void Scene::SpaceManipulation() //Only rotation can be manipulated before call in Render. Offset needs to be set in LoacActors.
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

void Scene::CollisionHandling(float dt)
{
	//Player & spawned objects
	auto max_i = Actor::spawnVector.size();
	
	for (auto &object : Actor::spawnVector) 
	{ 
		if (Collision::Intersect(uActorMap["player"], object)) 
		{
			score++;
			object->mMesh->~Mesh();
			object->mMesh->mEnableCollision = false;
		    std::cout << "Points earned: " << score << " off " << max_i << std:: endl;

			if (score == max_i)
			{
				std::cout << "Maximum points earned!!!: " << std::endl;
			}
		}
	}

	if (Collision::InvertIntersect(uActorMap["player"], uActorMap["testCube"],dt))
	{
		//std::cout << "Inverse Collision" << std::endl;
	}
}


