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


//Includes
#include "../Definitions.h"
#include "../MathLib/Vertex.h"

//External includes
#include <LASzip/dll/laszip_api.h>

struct MathLib
{

    //Spline Formulas _____________________________________________
    static float cox_deBoorFormula(int i, int p, float t, const std::vector<float>& knotVector)
    {

        ///Calculate Points on a B-Spline or Nurbs
       // i = index
       // p = degree
       // t = position also known as (X) in B(X)
       // vector of vector-knots

       //de-Boor`s recursive formula
        if (p == 0)
        {
            return (knotVector[i] <= t && t < knotVector[i + 1]) ? 1.0f : 0.0f;
        }
        else
        {
            float leftDenominator = knotVector[i + p] - knotVector[i];
            float rightDenominator = knotVector[i + p + 1] - knotVector[i + 1];

            float left = 0.0f;
            float right = 0.0f;

            if (leftDenominator != 0)
            {
                left = (t - knotVector[i]) / leftDenominator * cox_deBoorFormula(i, p - 1, t, knotVector);
            }

            if (rightDenominator != 0)
            {
                right = (knotVector[i + 1 + p] - t) / rightDenominator * cox_deBoorFormula(i + 1, p - 1, t, knotVector);
            }
            return left + right;
        }

    }

    static glm::vec3 EvaluateBSplineSurface(float u, float v, int du, int dv, const std::vector<float>& uKnot, const std::vector<float>& vKnot, const std::vector<std::vector<glm::vec3>>& controlPoints)
    {
        ///Evaluate points and attributes on a splineSurface
        // u & v = points evaluated
        // du & dv = degree of u and v
        // uKnot & vKnot = vectors cotaining the knot-vectors
        // controlpoints = vector of vectors containing glm::vec3-->vector on a 3D-axis

        glm::vec3 surfacePoint(0.0f);
        const size_t& numControlPointsU = controlPoints.size();
        const size_t& numControlPointsV = controlPoints[0].size() - 1;

        //Iterate over all existing points
        for (int i = 0; i < numControlPointsU; i++)
        {
            for (int j = 0; j < numControlPointsV; j++)
            {
                //calculates basisFunction based on the controlPoints influence in respect to U & V direction
                float splineEvaluation_i = cox_deBoorFormula(i, du, u, uKnot);
                float splineEvaluation_j = cox_deBoorFormula(j, dv, v, vKnot);

                //calculating the weight based on the sum of the basis functions using cox-deBoor`s algorithm
                glm::vec3 controlPointWeight = splineEvaluation_i * splineEvaluation_j * controlPoints[i][j];
                surfacePoint += controlPointWeight;
            }
        }
        //Returns a mathematical vector filled with wighted surface points
        return surfacePoint;
    }

    static glm::vec3 EvaluateBSplineSurfaceNormal(float u, float v, int du, int dv, int resU, int resV, const std::vector<float>& uKnot, const std::vector<float>& vKnot, const std::vector<std::vector<glm::vec3>>& controlPoints)
    {
        ///Evaluate points and attributes on a splineSurface and calculate the normal vector
        // u & v = points evaluated
        // du & dv = degree of u and v
        // resU & resV = resolution of the surface in the respective direction
        // uKnot & vKnot = vectors cotaining the knot-vectors
        // controlpoints = vector of vectors containing glm::vec3-->vector on a 3D-axis

        //Creating three vectors of surface points to be evaluated
        glm::vec3 splineX = EvaluateBSplineSurface(u, v, du, dv, uKnot, vKnot, controlPoints);
        glm::vec3 splineU = EvaluateBSplineSurface((u + 1.0f) / resU, v, du, dv, uKnot, vKnot, controlPoints);
        glm::vec3 splineV = EvaluateBSplineSurface(u, (v + 1.0f) / resV, du, dv, uKnot, vKnot, controlPoints);

        //Calculatin the tangents to find the normal-vectors
        glm::vec3 tangentU = splineU - splineX;
        glm::vec3 tangentV = splineV - splineX;

        glm::vec3 normal = glm::normalize(glm::cross(tangentU, tangentV));

        return normal;
    }

    //Random generator _____________________________________________
    template<typename T>
    static std::vector<T> CreateRandomKnotVector(const int& size, const float& min, const float& max)
    {
        std::vector<T> vectorKnot{ 0,0,0 };

        std::random_device rd;  // Obtain a random number from hardware
        std::mt19937 eng(rd()); // Seed the generator
        std::uniform_real_distribution<float> distr(min, max); // Define the range

        if (size <= 0 || min < 0 || max <= 0)
        {
            std::cout << "[WARNING]:size is less or equal to 0 \n";
            return {};
        }
        for (int i = 0; i < size; i++)
        {
            vectorKnot.emplace_back(distr(eng));
        }
        return vectorKnot;
    }

    static std::vector<std::vector<glm::vec3>> CreateKnotVectorTuple(size_t outerSize, size_t innerSize, const float& min, const float& max)
    {
        std::vector<std::vector<glm::vec3>> KnotTuple(outerSize, std::vector<glm::vec3>(innerSize));

        std::random_device rd;  // Obtain a random number from hardware
        std::mt19937 eng(rd()); // Seed the generator
        std::uniform_real_distribution<float> distr(min, max); // Define the range

        if (outerSize <= 0 || innerSize <= 0 || min < 0 || max <= 0)
        {
            std::cout << "[WARNING]:size is less or equal to 0 \n";
            return {};
        }
        for (size_t i = 0; i < outerSize; ++i)
        {
            for (size_t j = 0; j < innerSize; ++j)
            {
                KnotTuple[i][j] = glm::vec3((distr(eng)), (distr(eng)), (distr(eng)));
            }
        }
        return KnotTuple;
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
        ///Setting default values to zero
        _p1.y = 0.f;
        _p2.y = 0.f;
        _p3.y = 0.f;
        _playerPos.y = 0.f;
        glm::vec3 baryCoords{ 0.f,0.f,0.f };

        ///Calculating triangle surface area
        glm::vec3 u = { _p2 - _p1 }; //sw
        glm::vec3 v = { _p3 - _p1 }; //sw
        glm::vec3 n = glm::cross(u, v);
        float triangleSurfaceArea = n.y;

        ///Sub Triangle vectors
        glm::vec3 newU = { glm::cross(_p2 - _playerPos, _p3 - _playerPos) };
        glm::vec3 newV = { glm::cross(_p3 - _playerPos, _p1 - _playerPos) };
        glm::vec3 newW = { glm::cross(_p1 - _playerPos, _p2 - _playerPos) };

        ///Calculate area with respect to reverse clockwise direction
        //Sub Triangle 1 baryCoords X
        n = newU;
        baryCoords.x = n.y / triangleSurfaceArea;

        //Sub Triangle 2 baryCoords Y
        n = newV;
        baryCoords.y = n.y / triangleSurfaceArea;

        //Sub Triangle 3 baryCoords z
        n = newW;
        baryCoords.z = n.y / triangleSurfaceArea;

        return baryCoords;
    }

    template<typename T1, typename T2>
    static void BarycentricCoordinates(const std::shared_ptr<T1>& mMesh, const std::shared_ptr<T2>& mSurfaceMesh, float dt)
    {
        if (!mSurfaceMesh) { std::cout << "[WARNING]: No suitable surface selected \n"; return; };

        //vector of vertices and indices 
        for (size_t i = 0; i < mSurfaceMesh->mIndices.size(); i += 3)
        {
            //Assigning the values
            const unsigned int index1 = mSurfaceMesh->mIndices[i];
            const unsigned int index2 = mSurfaceMesh->mIndices[i + 1];
            const unsigned int index3 = mSurfaceMesh->mIndices[i + 2];

            //Collecting the postions of the indices 
            glm::vec3 point1 = mSurfaceMesh->mVertices[index1].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale();
            glm::vec3 point2 = mSurfaceMesh->mVertices[index2].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale();
            glm::vec3 point3 = mSurfaceMesh->mVertices[index3].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale();

            //Initialising variable which calculates bary-coords using the CalcBary-coords method
            glm::vec3 baryCoords = CalculateBarycentricCoordinates(point1, point2, point3, mMesh->GetLocalPosition());

            //Creating a variable which utilizes the calc-bary method
            float baryHeight = ((baryCoords.x * point1.y) + (baryCoords.y * point2.y + (mMesh->mExtent.y / 2)) + (baryCoords.z * point3.y));

            //If-checks if certain criterias are met folowing the rules for bary-coords behaviour
            if (baryCoords.x == 0 || baryCoords.y == 0 || baryCoords.z == 0)
            {
                mMesh->SetLocalPosition(mMesh->GetLocalPosition() + glm::vec3(0.01f, 0.f, 0.01f));
                baryCoords = CalculateBarycentricCoordinates(point1, point2, point3, mMesh->GetLocalPosition());
            }

            if (baryCoords.x > 0 && baryCoords.y > 0 && baryCoords.z > 0)
            {
                mMesh->SetLocalPosition(glm::vec3(mMesh->GetLocalPosition().x, baryHeight, mMesh->GetLocalPosition().z));
                //std::cout << "Bary coords works: " << std::endl;
                //std::cout << baryHeight << std::endl;
            }

        }
        return;


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
            glm::vec3 normal = glm::normalize(glm::cross(AB, AC));
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

};
