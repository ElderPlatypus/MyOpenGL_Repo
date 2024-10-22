#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <algorithm>


#include <vector>
#include <memory>


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

template<typename T>
std::vector<T> CreateRandomKnotVector(const int& size, const float& min, const float& max)
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