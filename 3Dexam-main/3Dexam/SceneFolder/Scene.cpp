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

	//uActorMap["cube"] = Actor::CreateCube();
	uActorMap["plane"] = Actor::CreatePlane(0, 2, 0, 5, 0.1f);

}

void Scene::LoadContent()
{
	LoadActors();
	mShader = new Shader("Shaders/Triangle.vs", "Shaders/Triangle.fs");
	
	SpaceManipulation();
}

void Scene::UnloadContent()
{
	for (auto actor : uActorMap)
	{
		actor.second->~Actor();
	}

	delete uActorMap["pyramid"];
	uActorMap["pyramid"] = nullptr;

	delete uActorMap["plane"];
	uActorMap["plane"] = nullptr;

	delete uActorMap["curve"];
	uActorMap["curve"] = nullptr;

	delete uActorMap["cube"];
	uActorMap["cube"] = nullptr;
}

void Scene::RenderScene(float dt, Transform globalTransform) 
{ 
	LoadContent();

	//Other uniforms
	glm::mat4 viewMat4 = glm::mat4(1.0);
	viewMat4 = glm::lookAt(glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.1f, 0.f));

	glm::mat4 pojectionMat4 = glm::mat4(1.0);
	pojectionMat4 = glm::perspective(glm::radians(45.f), (float)mWidth / mHeight, 0.1f, 100.f); 
	
	glm::mat4 perspectiveMat4 = glm::mat4(1.0); 
	perspectiveMat4 = glm::translate(perspectiveMat4, uActorMap["plane"]->GetLocalPosition());  
	perspectiveMat4 = glm::scale(perspectiveMat4, uActorMap["plane"]->GetLocalScale());  

	glm::mat4 rotationMatrix = glm::mat4_cast(uActorMap["plane"]->GetLocalRotation()); 
	perspectiveMat4 = perspectiveMat4 * rotationMatrix; 

	

	for (auto actor = uActorMap.begin(); actor!= uActorMap.end(); actor++) 
	{
		mShader->setMat4("model", perspectiveMat4); 
		mShader->setMat4("view", viewMat4);
		mShader->setMat4("projection", pojectionMat4);
		actor->second->drawActor(mShader);  
		globalTransform.SetTransformMatrix(globalTransform.GetTransformMatrix() * actor->second->GetLocalTransformMatrix());
	}

}

void Scene::SpaceManipulation()
{
	/*uActorMap["pyramid"]->SetLocalPosition(glm::vec3(0.5f, 0.f, 5.f));
	uActorMap["pyramid"]->SetLocalRotation(glm::vec3(0.f, float(glfwGetTime()), 0.f));*/
	uActorMap["plane"]->SetLocalPosition(glm::vec3(0.f, -3.0f, 8.f));
	uActorMap["plane"]->SetLocalRotation(glm::vec3(0.f, float(glfwGetTime()), 0.f)); 


}

