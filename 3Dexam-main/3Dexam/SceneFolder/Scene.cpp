#include "Scene.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/matrix_transform.hpp"


Scene::Scene(std::string name)
{
	mName = name;
}

void Scene::LoadActors()
{
	//Load Actors into U-Map
	//uActorMap["pyramid"] = Actor::CreatePyramid();
	//uActorMap["curve"] = Actor::CreateInterpolationCurve3Points(0, 13, 0.2f);

	uActorMap["cube"] = Actor::CreateCube();
	//uActorMap["plane"] = Actor::CreatePlane(0, 2, 0, 5, 0.1f);

	mSceneCamera = new Camera("SceneCamera", uActorMap["cube"]);
}

void Scene::LoadContent()
{
	LoadActors();
	mShader = new Shader("Shaders/Triangle.vs", "Shaders/Triangle.fs");
	
	SpaceManipulation();
}

void Scene::UnloadContent()
{
	for (auto actor = uActorMap.begin(); actor!= uActorMap.end(); actor++)
	{
		actor->second->~Actor(); 
	}

	delete uActorMap["pyramid"];
	uActorMap["pyramid"] = nullptr;

	delete uActorMap["plane"];
	uActorMap["plane"] = nullptr;

	delete uActorMap["curve"];
	uActorMap["curve"] = nullptr;

	delete uActorMap["cube"];
	uActorMap["cube"] = nullptr;

	delete mSceneCamera;
	mSceneCamera = nullptr;
}

void Scene::UpdateScene(float dt)
{
	mSceneCamera->UpdateCamera(dt); 
}

void Scene::RenderScene(float dt, Transform globalTransform) 
{ 
	LoadContent();
	BindCamera();
	UpdateScene(dt);

	//Other uniforms
	glm::mat4 viewMat4 = glm::mat4(1.0);
	//viewMat4 = glm::lookAt(glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.1f, 0.f));
	//viewMat4 = glm::lookAt(mSceneCamera->GetLocalPosition(), mSceneCamera->GetLocalPosition() + mSceneCamera->GetForwardVector(), glm::vec3(0.f, 1.f, 0.f));


	glm::mat4 pojectionMat4 = glm::mat4(1.0);
	pojectionMat4 = glm::perspective(glm::radians(45.f), (float)1920 / 1080, 0.1f, 100.f); 
	
	glm::mat4 perspectiveMat4 = glm::mat4(1.0); 
	perspectiveMat4 = glm::translate(perspectiveMat4, uActorMap["cube"]->GetLocalPosition());  
	perspectiveMat4 = glm::scale(perspectiveMat4, uActorMap["cube"]->GetLocalScale());  

	glm::mat4 rotationMatrix = glm::mat4_cast(uActorMap["cube"]->GetLocalRotation()); 
	perspectiveMat4 = perspectiveMat4 * rotationMatrix; 

	

	for (auto actor = uActorMap.begin(); actor!= uActorMap.end(); actor++) 
	{

		mShader->setMat4("model", perspectiveMat4);   

		mShader->setMat4("projection", pojectionMat4); 
		actor->second->drawActor(mShader,false);  
		globalTransform.SetTransformMatrix(globalTransform.GetTransformMatrix() * actor->second->GetLocalTransformMatrix()); 
	}

}

void Scene::SpaceManipulation()
{
	/*uActorMap["pyramid"]->SetLocalPosition(glm::vec3(0.5f, 0.f, 5.f));
	uActorMap["pyramid"]->SetLocalRotation(glm::vec3(0.f, float(glfwGetTime()), 0.f));*/
	uActorMap["cube"]->SetLocalPosition(glm::vec3(0.f, 0.0f, -8.f)); 

	uActorMap["cube"]->SetLocalRotation(glm::vec3((float)glfwGetTime(), (float)glfwGetTime(), (float)glfwGetTime()));
	

	/*std::cout << "camera local pos: " << mSceneCamera->GetLocalRotation().x << mSceneCamera->GetLocalRotation().y << mSceneCamera->GetLocalRotation().z << std::endl;
	std::cout << " \n";
	std::cout << "cube local rot: " << uActorMap["cube"]->GetLocalRotation().x << " " <<  uActorMap["cube"]->GetLocalRotation().y << " " << uActorMap["cube"]->GetLocalRotation().z << std::endl;
	std::cout << " \n";
	std::cout << "cube local pos: " << uActorMap["cube"]->GetLocalPosition().x << " " << uActorMap["cube"]->GetLocalPosition().y << " " << uActorMap["cube"]->GetLocalPosition().z << std::endl;*/

} 

void Scene::BindCamera()
{
	mShader->setMat4("view", mSceneCamera->GetViewMatrix());
	mShader->setMat4("projection", mSceneCamera->GetProjectionMatrix());
	mShader->setVec3("viewPos", mSceneCamera->GetLocalPosition());  
	//mShader->setMat4("model", mSceneCamera->GetLocalTransformMatrix());
}



