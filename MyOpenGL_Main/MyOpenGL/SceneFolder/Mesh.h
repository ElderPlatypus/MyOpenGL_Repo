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
	static std::shared_ptr<Mesh> CreateSplineSurface(int resU, int resV, int du, int dv,
		                                             const std::vector<float>& uKnot, const std::vector<float>& vKnot,
		                                             const std::vector<std::vector<glm::vec3>>& controlPoints, const float& size)
	{
		return BSplineSurfaceShape<Mesh>(resV, resV, du, dv, uKnot, vKnot, controlPoints, size); 
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

	///Transformation
    //---------------------------------Binders------------------------------------------ 
	VAO mVAO{ 0U };
	VBO mVBO{ 0U };
	EBO mEBO{ 0U };

	///Configure and draw Mesh
	void configureMesh(); //Binds VAO,VB & EBO to respective mesh
	void drawActor(const std::shared_ptr<Shader>& shader) const; 

	
	///Transformation
	//---------------------------------Members------------------------------------------ 
	std::vector<Vertex> mVertices{};
	std::vector<Index> mIndices{};
	std::string mName{};
	std::string mType{};

	std::shared_ptr<Transform> mTransform = std::make_shared<Transform>(); 

	//Bounding Box extent
	glm::vec3 mCenter{ 0.0f,0.0f,0.0f };
	glm::vec3 mExtent{ 0.0f,0.0f,0.0f };
	glm::vec3 minExtent{ 0.0f, 0.0f, 0.0f };
	glm::vec3 maxExtent{ 0.0f, 0.0f, 0.0f };

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
	int mTexType = 0; 
	GLDrawType drawType;
	std::shared_ptr<Shader> mShader{ nullptr }; 
	//---------------------------------Methods------------------------------------------
	void SetShader(const std::shared_ptr<Shader>& shader) { mShader = shader; }

	void TexConfig(const bool& useTexture, const int& texType)
	{
		mUseTex = useTexture;
		mTexType = texType; 
		mShader->use(); 
		mShader->setBool("useTex", useTexture);
		mShader->setInt("texType", mTexType);
	}


	///Lighting
	//---------------------------------Members------------------------------------------
	float mAmbientStrength = 1.0f;
	glm::vec3 mLightColor{ 0.0f, 1.0f, 0.0f };
	glm::vec3 mLightPos{ 0.f,100.f,0.f };
	glm::vec3 mObjectColor{ 1.f, 0.f, 0.0f };

	//Slope
	glm::vec3 slopeVec{ };
	glm::vec3 slopeColour{}; 
	float slope = 0.f;
	float averageSlope = 0.f;
	float normalizedSlope = 0.f;
	float slopeMagnitude = 0.0f;
	bool mUseLight = false;
	int mlightType = 0;
	//---------------------------------Default Shader------------------------------------------
	void SetShaderDefaults(bool useLight) const
	{
		mShader->setBool("useLight", useLight);
		mShader->setVec3("lightColor", glm::vec3(0.0f, 0.0f, 1.0f));
		mShader->setVec3("lightPos", glm::vec3(0.f, 100.f, 0.f));
		mShader->setVec3("objectColor", glm::vec3(1.f, 0.f, 0.0f));  
		mShader->setFloat("ambientStrength", 5.f);
	}
	//Slope:vertices
	float totalSlope = 0.0f;
	int counter = 0;
	float maxSlope = std::numeric_limits<float>::lowest();
	float minSlope = std::numeric_limits<float>::max();

	//---------------------------------Calculate slope based on derivatives------------------------------------------
	glm::vec3 CalculateSlopeColour()
	{

		ResetSlopeStatistics();
		//Iterating over each vertex to calculate current & next normal
		for (size_t i = 0; i < mVertices.size() - 1; i++)
		{
			const auto& currentHeight = mVertices[i].mPos.y;
			const auto& nextHeight = mVertices[i + 1].mPos.y;
			//std::cout << "[LOG]:Current Height:" << currentHeight << " Next Height:" << nextHeight << "\n";

			 //Calculate the horizontal distance (this is 2D distance on the X/Z plane)
			float distance = glm::distance(glm::vec2(mVertices[i].mPos.x, mVertices[i].mPos.z),
				                           glm::vec2(mVertices[i + 1].mPos.x, mVertices[i + 1].mPos.z));

			//std::cout << "[LOG]:Distance:" << distance << "\n";  

			//Calculatig vertical change
			float verticalChange = nextHeight - currentHeight; //Actual slope value
			//std::cout << "[LOG]:Vertical Change:" << verticalChange << "\n";
			
			//Condition to see if the slope ascend
			if (distance > 0.0f)
			{
				slope = verticalChange / distance; //Calculating the raw slope scalar
				//std::cout << "Vertical Change: " << verticalChange << ", Slope: " << slope << ", Slope Magnitude: " << distance << "\n";  
				totalSlope += slope;
				counter++;

				// Collect min/max slopes for normalization purposes
				minSlope = std::min(minSlope, slope);
				maxSlope = std::max(maxSlope, slope);

		/*		std::cout << "Counters: " << counter
					<< ", minSlope: " << minSlope
					<< ", maxSlope: " << maxSlope
					<< ", averageSlope: " << averageSlope
					<< ", normalizedSlope: " << normalizedSlope << "\n";
				std::cout.flush();*/
			}
			else
			{
				//std::cout << "Zero or negative distance detected at index " << i << "\n";
				continue; // Skip this iteration
			}
		}

		// After collecting slopes
		if (counter > 0)
		{
			averageSlope = totalSlope / counter;

			// Proper normalization handling
			if (maxSlope > minSlope)
			{
				normalizedSlope = (averageSlope - minSlope) / (maxSlope - minSlope);
			}
			else
			{
				// Use a default normalized slope if max and min are the same
				normalizedSlope = 0.0f;
			}

			// Clamp normalizedSlope between 0 and 1
			normalizedSlope = glm::clamp(normalizedSlope, 0.0f, 1.0f);
		}
		else
		{
			std::cout << "No valid slopes were calculated.\n";
			return mObjectColor;
		}
		//std::cout << "Normalised slope before proccesing:" << normalizedSlope << "\n";  

	    //Returing updated slopeColour vector
		return DetermineColor(normalizedSlope);
	}
	
	void ResetSlopeStatistics() 
	{
		totalSlope = 0.0f;
		counter = 0;
		maxSlope = std::numeric_limits<float>::lowest();
		minSlope = std::numeric_limits<float>::max();
	}

	//---------------------------------Determine colour according to the normalized slope------------------------------------------
	glm::vec3 DetermineColor(float normalizedSlope)
	{
		if (normalizedSlope < 0.10f) // Significant decline
		{
			std::cout << "Almost Flat: " << normalizedSlope << "\n";
			return glm::vec3(0.0f, 0.0f, 1.0f); // Blue
		}
		else if (normalizedSlope < 0.25f) // Mild decline
		{
			std::cout << "Mild Decline: " << normalizedSlope << "\n";
			return  MathLib::Lerp(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), normalizedSlope - 0.10f); // Blue-->Red
		}
		else if (normalizedSlope < 0.40f) // Moderate steep
		{
			std::cout << "Moderate Decline: " << normalizedSlope << "\n";
			return  MathLib::Lerp(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), normalizedSlope - 0.40f); // Red-->Green
		}
		else if (normalizedSlope < 0.55f) // Mild ascent
		{
			std::cout << "Mild Transition: " << normalizedSlope << "\n";
			return   MathLib::Lerp(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), normalizedSlope - 0.55f); // Green-->White
		}
		else if (normalizedSlope < 0.70f) // Mild ascent
		{
			std::cout << "Steep: " << normalizedSlope << "\n";
			return   MathLib::Lerp(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f), normalizedSlope - 0.70f); // White-->Purple
		}
		else if (normalizedSlope < 0.85f) // Mild ascent
		{
			std::cout << "Very Steept: " << normalizedSlope << "\n";
			return   MathLib::Lerp(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), normalizedSlope - 0.85f); // Purple-->Cyan
		}
		else // Significant ascent
		{
			std::cout << "Extreme Ascent: " << normalizedSlope << "\n";
			return  glm::vec3(1.0f, 0.0f, 0.0f); // Cyan
		}
	}

	//---------------------------------Light application------------------------------------------
	void LightConfig(const bool& useLight, const int& lightType) 
	{
		mUseLight = useLight;
		mlightType = lightType;

		if (mUseLight)
		{
			mShader->use();
			mShader->setBool("useLight", mUseLight);
			mShader->setVec3("lightColor", mLightColor);
			mShader->setVec3("lightPos", mLightPos);
			mShader->setFloat("ambientStrength", mAmbientStrength);

			switch (lightType) 
			{
			  case 0:
				  mShader->setVec3("objectColor", mObjectColor); 
				  break;
			  case 1:
			  {
				  slopeColour = CalculateSlopeColour(); 
			      mShader->setVec3("objectColor", slopeColour); 

				  break;
			  }
			  default:
				  SetShaderDefaults(mUseLight); 
				  return;
			} 
		}
		else
		{
			SetShaderDefaults(mUseLight);
		}
	}


	///ESC
	//---------------------------------Members------------------------------------------
	bool isPlayer = false;
	bool isActor = false;

};


