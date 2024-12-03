#pragma once

//Includes c++
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <string>
#include <memory>
#include <unordered_map>

//External Includes
#include <glm/gtc/matrix_transform.hpp>

//Full Inclusion
#include "../Definitions.h"
#include "../Shaders/Shader.h"
#include "../MathLib/Transform.h"
#include "../MathLib/Formulas.h"
#include "../MathLib/Shapes.h"
#include "../Utility/EnumArchive.h"

//Forawrd Declaring
struct Vertex;


class Mesh
{
public:
	Mesh(const std::string& type, const std::vector<Vertex>& vertices,
		 const std::vector<Index>& indices, const bool& useTex,
		 const GLDrawType& GLdrawType 
	);
	
	Mesh(const Mesh&) = delete;
	Mesh& operator = (const Mesh&) = delete;
	Mesh(const Mesh&&) = delete;
	Mesh& operator = (const Mesh&&) = delete;

	Mesh() = default;
	~Mesh();

	///Basic Shapes
	static std::shared_ptr<Mesh> CreateCube(float size)
	{
		return CubeShape<Mesh>(size);
	}
	static std::shared_ptr<Mesh> Create2DTriangle(float size)
	{
		return TriangleShape<Mesh>(size);
	}
	static std::shared_ptr<Mesh> CreatePyramid(float size)
	{
		return PyramidShape<Mesh>(size);
	}
	static std::shared_ptr<Mesh> CreateSphere(const int& stackCount, const int& sectorCount, const int& radius)
	{
		return SphereShape<Mesh>(stackCount, sectorCount, radius);
	}
	static std::shared_ptr<Mesh> CreateCircle(const glm::vec3& origin, const float& radius, const int& num_segment)
	{
		return CircleShape<Mesh>(origin, radius, num_segment);
	}
	
	///Planes & Curves
	static std::shared_ptr<Mesh> CreateInterpolationCurve3Points(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3,
		                                                         const float& startVal, const float& endingVal, const float& resolution)
	{
		return InterpolationCurve3PointsShape<Mesh>(p1, p2, p3, startVal, endingVal, resolution);
	}
	static std::shared_ptr<Mesh> CreatePlaneXZ(const float& xMin, const float& zMin, const float& xMax, const float& zMax, const float& resolution)
	{
		return PlaneXZShape<Mesh>(xMin, zMin, xMax, zMax, resolution);   
	}
	static std::shared_ptr<Mesh> CreatePlaneXY(const float& xMin, const float& yMin, const float& xMax, const float& yMax, const float& resolution)
	{
		return PlaneXYShape<Mesh>(xMin, yMin, xMax, yMax, resolution);
	}
	
	///Splines
	static std::shared_ptr<Mesh> CreateSplineSurface(const int& resU, const int& resV, const int& du, const int& dv, const std::vector<std::vector<glm::vec3>>& controlPoints, const float& size)
	{
		return BSplineSurfaceShape<Mesh>(resV, resV, du, dv, controlPoints, size); 
	}
	
	///LAS
	static std::shared_ptr<Mesh> CreatePointCloudFromLASFileSurface(const char* _fileDirectory, float _scaleFactor)
	{
		return PointCloudFromLas<Mesh>(_fileDirectory, _scaleFactor);
	}
	static std::shared_ptr<Mesh> CreateGridFromLas(const char* _fileDirectory, const float& _scaleFactor, const int& _resolution)
	{
		return TriangualtionGridFromLas<Mesh>(_fileDirectory, _scaleFactor, _resolution); 
	}

private:
	///Transformation
    //---------------------------------Binders------------------------------------------ 
	VAO mVAO{ 0U };
	VBO mVBO{ 0U };
	EBO mEBO{ 0U };

public:
	///Configure and draw Mesh
	void configureMesh(); //Binds VAO,VB & EBO to respective mesh
	void updateVertexAttribs();
	void drawActor(const std::shared_ptr<Shader>& shader) const; 

	///Transformation
	//---------------------------------Members------------------------------------------ 
	std::vector<Vertex> mVertices{};
	std::vector<Index> mIndices{};
	std::string mName{};
	std::string mType{};

	std::shared_ptr<Transform> mTransform = std::make_shared<Transform>(); 

	//Bounding Box extent
	glm::vec3 mCenter{ 0.0f, 0.0f, 0.0f };
	glm::vec3 mExtent{ 0.0f, 0.0f, 0.0f };
	glm::vec3 minExtent{ 0.0f, 0.0f, 0.0f };
	glm::vec3 maxExtent{ 0.0f, 0.0f, 0.0f };

	void UpdateBounds();

    //---------------------------------Methods Setters------------------------------------------------
	void SetTransformation(const std::shared_ptr<Transform>& transform)  { mTransform = transform; }  
	void SetLocalPosition(const glm::vec3& position) const { mTransform->SetPosition(position); }
	void SetLocalRotation(const glm::quat& rotation) const { mTransform->SetRotation(rotation); }
	void SetLocalScale(const glm::vec3& scale) const { mTransform->SetScale(scale); }
	void SetLocalTransformMatrix(const glm::mat4& transformMatrix) const { mTransform->SetTransformMatrix(transformMatrix); }
	//---------------------------------Methods Getters------------------------------------------
	std::shared_ptr<Transform> GetTransform() const { return mTransform; }  
	const glm::vec3& GetLocalPosition() const { return mTransform->GetPosition(); }
	const glm::quat& GetLocalRotation() const { return mTransform->GetOrientation(); } 
	const glm::vec3& GetLocalScale() const { return mTransform->GetScale(); } 

	glm::mat4 GetLocalTransformMatrix() const { return mTransform->GetTransformMatrix(); } 
	const glm::vec3 GetForwardVector() const { return mTransform->GetForwardVector(); }
	const glm::vec3 GetRightVector() const { return mTransform->GetRightVector(); }
	const glm::vec3 GetUpVector() const { return mTransform->GetUpVector(); }
	float GetPitch() const { return mTransform->GetPitch(); }
	float GetYaw() const { return mTransform->GetYaw(); } 


	///Textures
	//---------------------------------Members------------------------------------------
	bool mUseTex = false;
	TextureType mTexType{};
	GLDrawType drawType{};
	std::shared_ptr<Shader> mShader{ nullptr }; 

	//---------------------------------Methods------------------------------------------
	void SetShader(const std::shared_ptr<Shader>& shader) { mShader = shader; }
	void SetShaderDefaults(bool useLight) const;

	void TexConfig(const bool& useTexture, const TextureType& texType);
	void LightConfig(const bool& useLight, const LightType& lightType);

	///Lighting
	//---------------------------------Members------------------------------------------
	float mAmbientStrength = 1.0f;
	float mSpecularStrength = 0.5f;
	glm::vec3 mLightColor{ 1.0f, 1.0f, 1.0f };
	glm::vec3 mLightPos{ 0.f,100.f,0.f };
	glm::vec3 mObjectColor{ 1.f, 0.f, 0.0f };
	bool mUseLight = false;
	LightType mlightType{};


	//---------------------------------Calculate slope based on derivatives------------------------------------------
	float fric = 0.0f;
	glm::vec3 slopeColour;
	void CalculateSlopeColour();
	
	///ESC
	//---------------------------------Members------------------------------------------
	bool isPlayer = false;
	bool isActor = false;

};


