#pragma once
//Includes std
#include <iostream>
#include <vector>
#include <string>


//Includes glm
#include "Vertex.h"
#include "../Definitions.h"
#include "../Shaders/Shader.h"
#include "../Utility/Transform.h"
#include "../Shaders/Shader.h"
#include "../CameraFolder/Camera.h"

class Mesh
{
public:
	Mesh(const std::string& name, std::vector<Vertex>& vertices,
		std::vector<Index>& indices, const bool& useTex,
		const bool& drawLine = false
	);
	
	Mesh(const Mesh&) = delete;
	Mesh& operator = (const Mesh&) = delete;
	Mesh(const Mesh&&) = delete;
	Mesh& operator = (const Mesh&&) = delete;

	Mesh() = default;
	~Mesh();

	///Actor vertex data
	static Mesh* Create2DTriangle(float size);
	static Mesh* CreatePyramid(float size);
	static Mesh* CreateCube(float size);
	inline static float getSize;
	static Mesh* CreateInterpolationCurve3Points(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3,
		                                         const float& startVal, const float& endingVal, const float& resolution);
	static Mesh* CreatePlaneXZ(const float& xMin, const float& zMin, const float& xMax, const float& zMax, const float& resolution);
	static Mesh* CreatePlaneXY(const float& xMin, const float& yMin, const float& xMax, const float& yMax, const float& resolution);
	static Mesh* CreateSphere(const int& stackCount, const int& sectorCount, const int& radius);

	///Configure and Draw
	void configureMesh(); //Binds VAO,VB & EBO to respective mesh
	void drawActor(const Shader* shader)const;

	///Transformation
	//---------------------------------Members------------------------------------------ 
	std::vector<Vertex> mVertices{};
	std::vector<Index> mIndices{};
	std::string mName{};
	VAO mVAO{ 0U };
	VBO mVBO{ 0U };
	EBO mEBO{ 0U };
	Transform mTransform{};

	//Bounding Box extent
	glm::vec3 mCenter{ 0.0f,0.0f,0.0f };
	glm::vec3 mExtent{ 0.0f,0.0f,0.0f };
	glm::vec3 minExtent{ 0.0f, 0.0f, 0.0f };
	glm::vec3 maxExtent{ 0.0f, 0.0f, 0.0f };
	bool mEnableCollision = true;
    //---------------------------------Methods Setters------------------------------------------------
	void SetTransformation(const Transform& transform) { mTransform = transform; }
	void SetLocalPosition(const glm::vec3& position) { mTransform.SetPosition(position); }
	void SetLocalRotation(const glm::quat& rotation) { mTransform.SetRotation(rotation); }
	void SetLocalScale(const glm::vec3& scale) { mTransform.SetScale(scale); }
	void SetLocalTransformMatrix(const glm::mat4& transformMatrix) { mTransform.SetTransformMatrix(transformMatrix); }
	//---------------------------------Methods Getters------------------------------------------
	const Transform& GetTransform() const { return mTransform; }
	const glm::vec3& GetLocalPosition() const { return mTransform.GetPosition(); }
	const glm::quat& GetLocalRotation() const { return mTransform.GetOrientation(); }
	const glm::vec3& GetLocalScale() const { return mTransform.GetScale(); }
	const glm::mat4 GetLocalTransformMatrix() const { return mTransform.GetTransformMatrix(); }
	glm::vec3 GetRightVector() const { return mTransform.GetRightVector(); }

	///Textures
	//---------------------------------Members------------------------------------------
	bool mUseTex = false;
	bool mDrawLine = false;
	Shader* mShader{ nullptr };
	//---------------------------------Methods------------------------------------------
	void SetShader(Shader* shader) { mShader = shader; }
	void UseTexture(const bool& useBool)
	{
		mUseTex = useBool;
		mShader->setBool("useTex", useBool);
	}
	const bool& GetTexBool() const { return mUseTex; }

	///Lighting
	//---------------------------------Members------------------------------------------
	float mAmbientStrength = 1.0f;
	glm::vec3 mLightColor{ 1.f, 1.f, 1.f };
	glm::vec3 mLightPos{ 0.f,0.f,0.f };
	glm::vec3 mObjectColor{ 1.f,0.7f,0.2f };
	bool mUseLight = true;
	//---------------------------------Methods------------------------------------------
	void UseLight(const bool& useBool)
	{
		mUseLight = useBool;
		if (mUseLight == true)
		{
			mShader->use();
			mShader->setVec3("lightColor", mLightColor);
			mShader->setVec3("lightPos", mLightPos);
			mShader->setVec3("objectColor", mObjectColor);
		}
		else
		{
			mShader->use();
			mShader->setVec3("lightColor", glm::vec3(0.f));
			mShader->setVec3("lightPos", glm::vec3(0.f));
			mShader->setVec3("objectColor", glm::vec3(0.f));
		}
	}
	const bool& GetLightBool() const { return mUseLight; }



};

