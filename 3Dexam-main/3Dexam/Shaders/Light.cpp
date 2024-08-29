#include "Light.h"


Light::Light(Shader* shader) : mShader(shader)
{
	mShader->use();
	mShader->setVec3("lightColor", mLightColor); 
	mShader->setVec3("lightPos", mLightPos); 
	mShader->setVec3("objectColor", mObjectColor);
}

Light::Light(Shader* shader, Actor* actor) : mShader(shader), mActor(actor)
{
	mShader->use();
	mShader->setVec3("objectColor", mObjectColor);
	mShader->setVec3("lightColor", mLightColor);
	mShader->setVec3("lightPos", actor->GetLocalPosition());  
}

void Light::setObjectColor(glm::vec3 newObjectColor)
{
	mObjectColor = newObjectColor;
}

void Light::setAmbientStrength(float newAmbientStrength)
{
	mAmbientStrength = newAmbientStrength;
}

void Light::setLightColor(glm::vec3 newLightColor) 
{
	mLightColor = newLightColor;
}

