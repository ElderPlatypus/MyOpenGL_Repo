#include "Scene.h"

Scene::Scene(std::string name)
{
	mName = name;
}

///Loaders & Unloaders
void Scene::LoadActors()
{
	///Player
	/*uActorMap["player"] = new Actor(Mesh::CreateCube(2.0f),"player"); 
	uActorMap["player"]->mMesh->mUseTex = false;
	uActorMap["player"]->EnablePhysics = false;
	uActorMap["player"]->ExtrudeMesh(Extrude::increase, 2.0f); 
	uActorMap["player"]->mMesh->SetLocalPosition(glm::vec3(0, 10.f, 0.f));

	uActorMap["testCube1"] = new Actor(Mesh::CreateCube(2.0f), "TestCube");
	uActorMap["testCube1"]->EnablePhysics = false;*/


	///Curve
	//uActorMap["icurve"] = new Actor(Mesh::CreatePlaneXZ(-5, -5, 5, 5, 0.5),"Icurve");
	/*uActorMap["Icurve"]->mMesh->mUseTex = true;  
	uActorMap["player"]->SetBarySurfaceMesh(uActorMap["Icurve"]->mMesh);  */


	///Test cube
	uActorMap["testCube"] = new Actor(Mesh::CreateCube(2.0f),"TestCube"); 
	uActorMap["testCube"]->mMesh->mDrawLine = true;
	uActorMap["testCube"]->ExtrudeMesh(Extrude::increase, 30.0f);
	uActorMap["testCube"]->EnablePhysics = false;
	
	///Spawning vectors
	Actor::ActorType = 2;
	Actor::Spawner(100,-10.f,10.f);

	for (auto &object : Actor::spawnVector) 
	{
	  uActorMap.emplace(object->mName,object);
	}

	///Create camera object
    mSceneCamera = new Camera("SceneCamera");

	/*for (auto& [name, it] : uActorMap)
	{
		it->EnablePhysics = false;
	}*/
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
		Collision::InvertIntersect(object, uActorMap["testCube"], dt);
		/*for (auto& another : Actor::spawnVector)
		{
			if (object != another)
			{
			  Collision::Intersect(object, another);
			}
		}*/
		
	}
}


