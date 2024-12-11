#pragma once

//Glm include
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <algorithm>

//Includes c++
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include <random>


//Includes
#include "../Definitions.h"
#include "../MathLib/Vertex.h"


//External includes
#include <LASzip/dll/laszip_api.h>

class Actor;
struct MathLib
{

    //Spline Formulas _____________________________________________
    static float NURBS(size_t i, size_t j, float t, const std::vector<float>& knotVector)    
    {
        ///Calculate Points on a B-Spline or Nurbs
        // i = index
        // p = degree
        // t = position also known as (X) in B(X)
        // vector of vector-knots

        //Base Case of degree 0 | Basis Function
        if (j == 0)
        {
            return (knotVector[i] <= t && t < knotVector[i + 1]) ? 1.0f : 0.0f;
        }
       
        if (i + j >= knotVector.size() || i + j + 1 >= knotVector.size())
        {
            //Out of bounds
            return 0.0f;
        }

        //Recursive case | calculation of degree > 0
        float leftDenominator = knotVector[i + j] - knotVector[i];
        float rightDenominator = knotVector[i + j + 1] - knotVector[i + 1];

        float leftSide = 0.0f;
        float rightSide = 0.0f;

        if (leftDenominator != 0)
        {
            leftSide = (t - knotVector[i]) / leftDenominator * NURBS(i, j - 1, t, knotVector);
        }

        if (rightDenominator != 0)
        {
            rightSide = (knotVector[i + 1 + j] - t) / rightDenominator * NURBS(i + 1, j - 1, t, knotVector);
        }
        return leftSide + rightSide;
    }

    static glm::vec3 EvaluateBSplineSurface(const float& u, const float& v, const int& degreeU, const int& degreeV, const std::vector<std::vector<glm::vec3>>& controlPoints)
    {
        ///Evaluate points and attributes on a splineSurface
        // u & v = points evaluated
        // i & j = degree of u and v
        // uKnot & vKnot = vectors cotaining the knot-vectors
        // controlpoints = vector of vectors containing glm::vec3-->vector on a 3D-axis
        if (controlPoints.empty() || controlPoints[0].empty())
        {
            //Default return value
            return glm::vec3(0.f);
        }

        //Calculate knot vectors
        const auto& data = CalculateKnotVector(degreeU, degreeV); 
        std::vector<float> knotVectorU = data.first;
        std::vector<float> knotVectorV = data.second; 

        //Initliasining surfacePoints
        glm::vec3 surfacePoint(0.0f);
        const size_t& numControlPointsU = controlPoints.size(); //Direction X
        const size_t& numControlPointsV = controlPoints[0].size(); //Direction Y

        //Initialzing weight for normalization
        float totWeight = 0.0f;

        //Iterate over all existing points
        for (size_t i = 0; i < numControlPointsU; i++) 
        {
            for (size_t j = 0; j < numControlPointsV; j++)
            {
                //calculates basisFunction based on the controlPoints influence in respect to U & V direction
                float basisU = NURBS(i, degreeU, u, knotVectorU); 
                float basisV = NURBS(j, degreeV, v, knotVectorV); 

                //Calculating current weight
                float currentWeight = basisU * basisV;

                //calculating the weight based on the sum of the basis functions using cox-deBoor`s algorithm
                surfacePoint += currentWeight * controlPoints[i][j];
                totWeight += currentWeight;
            }
        }
        //Normalizing surface point if not zero
        if (totWeight > 0.0f)
        {
            surfacePoint /= totWeight; 
        }

        //Returns a mathematical vector filled with weighted surface points
        return surfacePoint;
    }

    static glm::vec3 EvaluateBSplineSurfaceNormal(const float& u, const float& v, const int& degreeU, const int& degreeV, const int& resU, const int& resV, const std::vector<std::vector<glm::vec3>>& controlPoints)
    {
        ///Evaluate points and attributes on a splineSurface and calculate the normal vector
        // u & v = points evaluated
        // i & j = degree of u and v
        // resU & resV = resolution of the surface in the respective direction
        // uKnot & vKnot = vectors cotaining the knot-vectors
        // controlpoints = vector of vectors containing glm::vec3-->vector on a 3D-axis

        float epsilon = 0.001f;

        //Creating three vectors of surface points to be evaluated
        glm::vec3 splineX = EvaluateBSplineSurface(u, v, degreeU, degreeV, controlPoints); 
        glm::vec3 splineU = EvaluateBSplineSurface(u + epsilon , v, degreeU, degreeV, controlPoints);
        glm::vec3 splineV = EvaluateBSplineSurface(u, v + epsilon , degreeU, degreeV, controlPoints); 

        //Calculating the tangents to find the normal-vectors
        glm::vec3 tangentU = splineU - splineX;
        glm::vec3 tangentV = splineV - splineX;

        //Close to zero handler
        if (glm::length(tangentU) < 1e-6f || glm::length(tangentV) < 1e-6f)
        {
            return glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
        }

        //Calulate unity vector
        glm::vec3 normal = glm::normalize(glm::cross(tangentU, tangentV));

        return normal;
    }

    //Random generator _____________________________________________
    static std::pair<std::vector<float>, std::vector<float>> CalculateKnotVector(const size_t& numControlPointsU, const size_t& numControlPointsV)
    {

        //Degree of the spline
        size_t degreeU = 2;
        size_t degreeV = 2; 

        size_t sizeU = numControlPointsU + degreeU + 1;
        size_t sizeV = numControlPointsV + degreeV + 1;  

        std::vector<float> vectorKnotU(sizeU); //Dir X
        std::vector<float> vectorKnotV(sizeV); //Dir Y

        //Assume this for explenation: numControlpoints = 5, degree = 3
        for (size_t k = 0; k  < sizeU; k++) 
        {
            if (k < degreeU) // 0 < 3 --> vectorKnot[0] = 0.0f
            {
                vectorKnotU[k] = 0.0f; 
            } 
            else if(k >= degreeU && k < sizeU - degreeU) // 4  >=  degreeU && 4 < sizeU - degreeU --> 4 >= 3 && 4 < 6 --> vectorKnot[4] = 4 - 3 = 1
            {
                vectorKnotU[k] = (float)(k - degreeU); 
            }
            else  // 6 >= 9 - 3 <==> 6 >= 6 --> vectorKnot[6] = 5 - 3 = 2
            {
                vectorKnotU[k] = (float)(numControlPointsU - degreeU);
            }
        }

        for (size_t k = 0; k < sizeV; k++)
        {
            if (k < degreeV)
            {
                vectorKnotV[k] = 0.0f;
            }
            else if (k >= degreeV && k < sizeV - degreeV)  
            {
                vectorKnotV[k] = (float)(k - degreeV);
            }
            else
            {
                vectorKnotV[k] = (float)(numControlPointsV - degreeV); 
            }
        }

        return std::make_pair(vectorKnotU,vectorKnotV);
    }

    static std::vector<std::vector<glm::vec3>> CreateUniformControlPoints(const int& numU, const int& numV, const float& min, const float& max)
    {

        if (numU <= 0 || numV <= 0)
        {
            std::cerr << "[WARNING]: Size must be greater than 0\n";
            return {};
        }
        std::vector<std::vector<glm::vec3>> controlPoints(numU, std::vector<glm::vec3>(numV));

        //Populate controlPoints
        for(int i = 0; i < numU; i++)     
        {
            for(int j = 0; j < numV; j++) 
            {
                float x = i * (max - min) / (numU - 1) + min; 
                float z = j * (max - min) / (numV - 1) + min; 
                float y = std::sin(x) * std::cos(z) * (max - min) / 2.0f;  

                controlPoints[i][j] = glm::vec3(x, y, z); 
            }
        }
        return controlPoints; 
    }

    template <typename T>
    static std::vector<std::vector<glm::vec3>> CreateControlPointsFromActor(const float& min, const float& max, const std::shared_ptr<T>& actor) 
    {

        if (actor->getMesh()->mVertices.size() <= 1)
        {
            std::cerr << "[WARNING]: Size must be greater than 1\n";
            return { {}, {} };
        }

        //Number of control points. Using square root since grid is of second degree
        int totalVertices = (int)actor->getMesh()->mVertices.size(); 
        int numControlPointsX = (int)(glm::sqrt(totalVertices)); 
        int numControlPointsY = totalVertices/numControlPointsX ;

        if (numControlPointsX * numControlPointsY != totalVertices) 
        {
            return { {}, {} };
        }

        //2D Vetor to populate with control points
        std::vector<std::vector<glm::vec3>> controlPoints(numControlPointsX, std::vector<glm::vec3>(numControlPointsY)); 

        //Populate control points 
        for (int i = 0; i < numControlPointsX; i++)
        {
            for (int j = 0; j < numControlPointsY; j++)  
            {
                //Access corresponding vertex
                int vertexIndex = i * numControlPointsY + j; 
                controlPoints[i][j] = glm::vec3(actor->getMesh()->mVertices[vertexIndex].mPos);
            }
        }
        return controlPoints;
    }
   
    
    //Las Loader _____________________________________________
    static std::pair <std::vector<Vertex>, std::vector<Index>> LoadLAS_Data(const char* _fileDirectory, float _scaleFactor)
    {
        // create laszip pointer and reader
        laszip_POINTER laszip_pointer;
        if (laszip_create(&laszip_pointer)) {}
        if (!laszip_pointer) { std::cout << "[WARNING]:laszip_POINTER Not found\n"; return {}; }

        // open the reader if contet is compressed
        laszip_BOOL is_compressed;
        if (laszip_open_reader(laszip_pointer, _fileDirectory, &is_compressed)) {}
        if (!is_compressed) { std::cout << "[WARNING]:laszip_BOOL Not found\n"; return {}; }

        // get the pointer to the header that was opened for reading if it exists
        laszip_header* header = nullptr;
        if (laszip_get_header_pointer(laszip_pointer, &header)) {}
        if (!header) { std::cout << "[WARNING]:laszip_HEADER Not found\n"; return {}; }

        // get the pointer to the points that will be read
        laszip_point* laszip_point;
        if (laszip_get_point_pointer(laszip_pointer, &laszip_point)) {}
        if (!laszip_point) { std::cout << "[WARNING]:laszip_POINTER Not found\n"; return {}; }

        // display path
        std::cout << "[LOG]:Reading from file:" << _fileDirectory << "\n";

        // how many points does the file have
        laszip_I64 numTotalPoints = (header->number_of_point_records ? header->number_of_point_records : header->extended_number_of_point_records);
        std::cout << "[LOG]:Number of Points in file is:" << numTotalPoints << "\n";

        // adding points to temp vertex
        std::vector<Vertex> tempVertices;
        for (int i = 0; i < numTotalPoints; i++)
        {
            if (laszip_read_point(laszip_pointer)) {}
            // Apply  costum scaling when reading point coordinates
            float x = (float)(laszip_point->X * header->x_scale_factor + header->x_offset) * _scaleFactor;
            float y = (float)(laszip_point->Y * header->y_scale_factor + header->y_offset) * _scaleFactor;
            float z = (float)(laszip_point->Z * header->z_scale_factor + header->z_offset) * _scaleFactor;

            glm::vec3 colorf = glm::vec3(0);
            glm::vec3 vertPos = glm::vec3(x, z, y);

            tempVertices.emplace_back(Vertex(vertPos, glm::vec3(1.f, 1.f, 1.f), vertPos));
        }

        //creating the actual vertex vector for repositioning
        std::vector<Vertex> newVertices;
        std::vector<Index> indices;
        if (!tempVertices.empty())
        {
            glm::vec3 MinVert = tempVertices[0].mPos;
            glm::vec3 MaxVert = tempVertices[0].mPos;

            //Finding minimum and maximum values 
            for (const auto& vertex : tempVertices)
            {
                MinVert = glm::min(MinVert, vertex.mPos);
                MaxVert = glm::max(MaxVert, vertex.mPos);
            }

            //calculating half the length of position
            glm::vec3 midPoint = (MinVert + MaxVert) / glm::vec3{ 2.f };

            //adding updated vertices to vector
            int counter = 0;
            int indicesIncrement = 0;

            for (auto& vertex : tempVertices)
            {

                vertex.mPos -= midPoint;
                newVertices.emplace_back(vertex.mPos.x, vertex.mPos.y, vertex.mPos.z);

                if (counter == 3)
                {
                    indices.emplace_back(indicesIncrement - 1);
                    counter = 0;
                }
                indices.emplace_back(indicesIncrement);

                counter++;
                indicesIncrement++;
            }


            //Calculate normals
            auto normals = CalculateNormalVector(newVertices, indices);
            for (size_t index = 0; index < newVertices.size(); index++)
            {
                newVertices[index].mNormals = normals[index];
                //std::cout << "[LOG]:Normal Values X:" << normals[index].x << " Y:" << normals[index].y << " Z:" << normals[index].z <<"\n";
            }
            std::cout << "[LOG]:Loaded:(" << (float)(indicesIncrement * 100) / tempVertices.size() << "%)\n\n";
        }

        // close the reader and destroy it when done reading
        if (laszip_close_reader(laszip_pointer)) {}
        if (laszip_destroy(laszip_pointer)) {}

        return std::make_pair(newVertices, indices);
    }


    //Barycentric coordinate handler _____________________________________________
    static glm::vec3 CalculateBarycentricCoordinates(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3, glm::vec3 _playerPos)
    {
        //Setting default values to zero
        glm::vec3 baryCoords = {0.0f,0.0f,0.0f};

        //Calculating triangle surface area
        glm::vec3 u =  _p2 - _p1; //sw
        glm::vec3 v =  _p3 - _p1; //sw
        glm::vec3 n = glm::cross(u, v);
        float triangleSurfaceArea = glm::length(n); 

        if (triangleSurfaceArea == 0.0f)
        {
            return baryCoords;
        }

        //Sub Triangle vectors
        glm::vec3 newU =  glm::cross(_p2 - _playerPos, _p3 - _playerPos);
        glm::vec3 newV =  glm::cross(_p3 - _playerPos, _p1 - _playerPos);
        glm::vec3 newW =  glm::cross(_p1 - _playerPos, _p2 - _playerPos);
     

        //Calculate area for sub triangles reverse clock-wise
        baryCoords.x = newU.y / triangleSurfaceArea;
        baryCoords.y = newV.y / triangleSurfaceArea;
        baryCoords.z = newW.y / triangleSurfaceArea;

        // Normalizing the barycentric coordinates
        float barySum = baryCoords.x + baryCoords.y + baryCoords.z;
        if (barySum > 0.0f)
        {
            baryCoords /= barySum;
        }
        return baryCoords;
    }

    template <typename T1, typename T2>
    static void DoBarycentricCoordinatesMesh(const std::shared_ptr<T1>& mMesh, const std::shared_ptr<T2>& mSurfaceMesh, float dt)
    {
        if (!mSurfaceMesh) { std::cout << "[WARNING]: No suitable surface selected \n"; return; };

        //vector of vertices and indices 
        for (size_t i = 0; i < mSurfaceMesh->mIndices.size(); i += 3)
        {
            //Collecting the postions of the indices 
            glm::vec3 point1 = mSurfaceMesh->mVertices[mSurfaceMesh->mIndices[i]].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale(); 
            glm::vec3 point2 = mSurfaceMesh->mVertices[mSurfaceMesh->mIndices[i + 1]].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale(); 
            glm::vec3 point3 = mSurfaceMesh->mVertices[mSurfaceMesh->mIndices[i + 2]].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale(); 

            //Initialising variable which calculates bary-coords using the CalcBary-coords method
            glm::vec3 baryCoords = CalculateBarycentricCoordinates(point1, point2, point3, mMesh->GetLocalPosition());

            //If-checks if certain criterias are met folowing the rules for bary-coords behaviour
            if (baryCoords.x > 0 && baryCoords.y > 0 && baryCoords.z > 0)
            {
                float baryHeight = (baryCoords.x * point1.y) + (baryCoords.y * point2.y) + (baryCoords.z * point3.y);
                mMesh->SetLocalPosition(glm::vec3(mMesh->GetLocalPosition().x, baryHeight + mMesh->mExtent.y, mMesh->GetLocalPosition().z));
                break;
            }
        }
    }

    template <typename T1, typename T2>
    static void DoBarycentricCoordinatesActor(const std::shared_ptr<T1>& mActor, const std::shared_ptr<T2>& mSurfaceMesh, float dt)
    {
        if (!mSurfaceMesh) { std::cout << "[WARNING]: No suitable surface selected \n"; return; };

        //vector of vertices and indices 
        for (size_t i = 0; i < mSurfaceMesh->mIndices.size(); i += 3)
        {
            //Collecting the postions of the indices 
            glm::vec3 point1 = mSurfaceMesh->mVertices[mSurfaceMesh->mIndices[i]].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale();
            glm::vec3 point2 = mSurfaceMesh->mVertices[mSurfaceMesh->mIndices[i + 1]].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale();
            glm::vec3 point3 = mSurfaceMesh->mVertices[mSurfaceMesh->mIndices[i + 2]].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale();

            //Initialising variable which calculates bary-coords using the CalcBary-coords method
            glm::vec3 baryCoords = CalculateBarycentricCoordinates(point1, point2, point3, mActor->rigidB->GetLocalPosition());

            //If-checks if certain criterias are met folowing the rules for bary-coords behaviour
            if (baryCoords.x > 0 && baryCoords.y > 0 && baryCoords.z > 0)
            {
                float baryHeight = (baryCoords.x * point1.y) + (baryCoords.y * point2.y) + (baryCoords.z * point3.y);
                mActor->rigidB->SetLocalPosition(glm::vec3(mActor->rigidB->GetLocalPosition().x, baryHeight + mActor->getMesh()->mExtent.y, mActor->rigidB->GetLocalPosition().z));
                break;
            }
        }
    }

    //Usefull Mathematical Formulas _____________________________________________
    template<typename T>
    static glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, T t)
    {
        //Lerp formula
        return a + t * (b - a);
    }

    static std::vector<glm::vec3> CalculateNormalVector(const std::vector<Vertex>& vertexArray, const std::vector<Index>& indexArray)
    {
        std::vector<glm::vec3> newNormals;
        newNormals.resize(vertexArray.size(), glm::vec3(0.f));
  
        // Ensure there are at least 3 vertices to form a triangle
        if (indexArray.size() < 3)
        {
            return newNormals;
        }

        for (size_t i = 0; i < indexArray.size(); i += 3)
        {
            size_t index0 = indexArray[i];
            size_t index1 = indexArray[i + 1];
            size_t index2 = indexArray[i + 2];

            glm::vec3 A = vertexArray[index0].mPos;
            glm::vec3 B = vertexArray[index1].mPos;
            glm::vec3 C = vertexArray[index2].mPos;

            glm::vec3 AB = B - A;
            glm::vec3 AC = C - A;
            glm::vec3 normal = glm::cross(AB, AC);
            float weight = glm::length(AC) * glm::length(AB);


            newNormals[index0] += normal * weight; 
            newNormals[index1] += normal * weight;
            newNormals[index2] += normal * weight;
        }

        for (size_t i = 0; i < newNormals.size(); ++i)
        {
            newNormals[i] = glm::normalize(newNormals[i]);
        }
        return newNormals;
    }


    //Hash maps
    template<typename T1, typename T2>
    struct PairHash
    {
         std::size_t operator() (const std::pair<T1, T2>& pair) const
        {
            auto hash1 = std::hash<T1>{}(pair.first);
            auto hash2 = std::hash<T2>{}(pair.second);
            return hash1 ^ (hash2 << 1);;  
        }
    };

    struct GridHash
    {
        std::size_t operator() (const glm::ivec3& key) const
        {
            return std::hash<int>()(key.x) ^ (std::hash<int>()(key.y) << 1) ^ (std::hash<int>()(key.z) << 2);
        }
    };


    //Friction
    template<typename T>
    static float CalculateFriction(const std::shared_ptr<T>& actor)
    {
        float fric = 0.0f; 
        //Initialising local variables
        float min = 0;
        float max = 0;
        float average = 0;
        for (size_t i = 0; i < actor->mVertices.size(); i++)
        {
            //Initialising the variable for the ground normal
            glm::vec3 groundNormal(0.f, 1.f, 0.f);

            //Calcuating dot product and 
            float dotProduct = glm::dot(actor->mVertices[i].mNormals, groundNormal);
            float magnitude = glm::length(actor->mVertices[i].mNormals) * glm::length(groundNormal);

            //Convertig angle to radian
            const float radianConvert = 180.f / 3.14f;

            //Using arccos to find angle on slope and converting to radian
            float angle = glm::atan(dotProduct / magnitude) * radianConvert;     

            float force = angle * glm::cos(angle) * 9.81f; 
            //Calulating min and max and average angle
            min = glm::min(min, angle);
            max = glm::max(max, angle);
            average = (min + max) / 2.0f;
            fric = force; 
        }
        return fric;
    }

    //Ransom Generators
    static glm::vec3 CreateRandomVec3(const float& distribMin, const float& distribMax)
    {
        std::random_device rd;
        std::mt19937 eng(rd());
        std::uniform_real_distribution<float> random(distribMin, distribMax); 
        glm::vec3 randVec(random(eng), random(eng), random(eng));
        return randVec;
    }

};
