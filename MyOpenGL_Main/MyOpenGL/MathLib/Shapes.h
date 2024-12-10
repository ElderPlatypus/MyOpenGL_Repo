#pragma once

//External Includes
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

//Includes c++
#include <chrono>
#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <utility>
#include <unordered_map>


//Full incluson
#include "../Definitions.h"
#include "../MathLib/Formulas.h"
#include "../Physics/Collision.h"
#include "../Utility/EnumArchive.h"

//Forward Declaring
struct Vertex;

//Basic Shapes ____________________________________________
template <typename T>
std::shared_ptr<T> CubeShape(float size)
{
    std::vector<Vertex> vertices = {
        // Front face
        {{-size, -size,  size}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // Bottom-left    
        {{ size, -size,  size}, {0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}}, // Bottom-right  
        {{ size,  size,  size}, {0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // Top-right 
        {{-size,  size,  size}, {0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}}, // Top-left
        // Back face
        {{-size, -size, -size}, {0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
        {{ size, -size, -size}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
        {{ size,  size, -size}, {0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},
        {{-size,  size, -size}, {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
        // Left face
        {{-size, -size, -size}, {-1.0f,  0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-size, -size,  size}, {-1.0f,  0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-size,  size,  size}, {-1.0f,  0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-size,  size, -size}, {-1.0f,  0.0f, 0.0f}, {0.0f, 1.0f}},
        // Right face
        {{ size, -size, -size}, {1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
        {{ size, -size,  size}, {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
        {{ size,  size,  size}, {1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
        {{ size,  size, -size}, {1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
        // Top face
        {{-size,  size, -size}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},
        {{-size,  size,  size}, {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
        {{ size,  size,  size}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
        {{ size,  size, -size}, {0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
        // Bottom face
        {{-size, -size, -size}, {0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
        {{-size, -size,  size}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},
        {{ size, -size,  size}, {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
        {{ size, -size, -size}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}
    };

    std::vector<Index> indices = {
        // Front face
        0, 1, 2, 0, 2, 3,
        // Back face
        4, 5, 6, 4, 6, 7,
        // Left face
        8, 9, 10, 8, 10, 11,
        // Right face
        12, 13, 14, 12, 14, 15,
        // Top face
        16, 17, 18, 16, 18, 19,
        // Bottom face
        20, 21, 22, 20, 22, 23
    };
    return std::make_shared<T>("Cube", vertices, indices, true, GLDrawType::Triangle);
};


template <typename T>
std::shared_ptr<T> FlatPlaneXZShape(float size)
{
    std::vector<Vertex> vertices = {
    { { -size, 0.f, -size}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f} }, // Bottom-left    
    { {  size, 0.f, -size}, {0.0f,  0.0f,  1.0f}, {1.0f, 0.0f} }, // Bottom-right  
    { {  size, 0.f,  size}, {0.0f,  0.0f,  1.0f}, {1.0f, 1.0f} }, // Top-right 
    { { -size, 0.f,  size}, {0.0f,  0.0f,  1.0f}, {0.0f, 1.0f} }, // Top-left
    };

    std::vector<Index> indices = {
        // Front face
        0, 1, 2, 0, 2, 3
    };


    return std::make_shared<T>("Square", vertices, indices, true, GLDrawType::Triangle);
};


template <typename T>
std::shared_ptr<T> TriangleShape(float size)
{
    std::vector<Vertex> vertices =
    {
        // positions           // colors           //textures
       {{ size, -size, size},  {size, 0.0f, 0.0f}, {1.0f,0.0f}},  // bottom right
       {{-size, -size, size},  {0.0f, 1.0f, 0.0f}, {0.0f,0.0f}},  // bottom left
       {{ size,  size, size},  {0.0f, 0.0f, 1.0f}, {1.0f,1.0f}}   // top 
    };

    std::vector<Index> indices =
    {
        0,1,2
    };

    return std::make_shared<T>("2DTriangle", vertices, indices, true, GLDrawType::Triangle);
};

template <typename T>
std::shared_ptr<T> PyramidShape(float size)
{
    std::vector<Vertex> vertices =
    {
        // positions              // normals           //textures
        //Bottom Square Indices(0-3)
        {{ -size, -size, -size},  {0.0f, -1.0f, 0.0f}, {0.f,1.f}},  // bottom closeLeft | Vertex(0)
        {{  size, -size, -size},  {0.0f, -1.0f, 0.0f}, {1.f,1.f}},  // bottom closeRight | Vertex(1)
        {{  size, -size, size},   {0.0f, -1.0f, 0.0f}, {1.f,0.f}},  // bottom farRight | Vertex(2)
        {{ -size, -size, size},   {0.0f, -1.0f, 0.0f}, {0.f,0.f}},  // bottom farLeft | Vertex(3)

        //_________________________front & back-face_____________________
        //Front Triangle Indices(4-6)
        {{  size,  size,  size},  {0.0f,  0.0f, 0.0f}, {0.5f,1.0f}},  // top centre | Vertex(4)
        {{ -size, -size, -size},  {0.0f, -1.0f, 0.0f}, {0.0f,0.0f}},   // bottom closeLeft | Vertex(5)
        {{  size, -size, -size},  {0.0f, -1.0f, 0.0f}, {1.0f,0.0f}},   // bottom closeRight | Vertex(6)

        //Back Triangle Indices(7-9)
        {{  size,  size, size},  {0.0f,  0.0f, 0.0f}, {0.5f,1.0f}},  // top centre | Vertex(7)
        {{ -size, -size, size},  {0.0f, -1.0f, 0.0f}, {0.0f,0.0f}},   // bottom farLeft | Vertex(9)
        {{  size, -size, size},  {0.0f, -1.0f, 0.0f}, {1.0f,0.0f}},   // bottom farRight | Vertex(8)

        //_________________________left & right-face_____________________
        //Right Triangle Indices(10-12)
        {{  size,  size,  size},  {0.0f,  0.0f, 0.0f}, {0.05,1.0f}},  // top centre | Vertex(10)
        {{ -size, -size,  size},  {0.0f, -1.0f, 0.0f}, {1.0f,0.0f}},   // bottom farLeft | Vertexsize
        {{ -size, -size, -size},  {0.0f, -1.0f, 0.0f}, {0.0f,0.0f}},   // bottom closeLeft | Vertex(1size        //Left Triangle Indices(13-15)
        {{  size,  size,  size},  {0.0f,  0.0f, 0.0f}, {0.5f,1.0f}},  // top centre | Vertex(size        {{  0.5f, -0.5f, 0.5f},   {0.0f, -1.0f, 0.0f}, {1.f,0.f}},   // bottom farRight | Vertex(2)
        {{  size, -size, -size},  {0.0f, -1.0f, 0.0f}, {0.0f,0.0f}}    // bottom closeRight |sizetex(1)
    };

    std::vector<Index> indices =
    {
        //Bottom face
        0,1,2, 0,2,3,

        //Left Face
        4,5,6,

        //Right Face 
        7,8,9,

        //Front Face
        10,11,12,

        //Back Face
        13,14,15
    };

    return std::make_shared<T>("Pyramid", vertices, indices, true, GLDrawType::Triangle);

};

template <typename T>
std::shared_ptr<T> SphereShape(const int& stackCount, const int& sectorCount, const int& radius)
{
    float PI = 3.14159265358979323846f;
    std::vector<Vertex> vertices;
    std::vector<Index> indices;

    //uniform variables
    float x, y, z, xy; //position
    float nx, ny, nz, invertLength = 1.0f / radius; //normal
    float s, t; //textCoord

    //sphere segment logic
    float sectorStep = (2.f * PI / sectorCount); //Y->direction sub-divisons
    float stackStep = (PI / stackCount); //X->direction sub-divisons
    float sectorAngle, stackAngle; //angle variables for y|x

    for (int i = 0; i <= stackCount; i++)
    {
        stackAngle = static_cast<float>(PI / 2.f - i * stackStep); //pi->-pi domain 
        xy = radius * glm::cos(stackAngle); // cos 
        z = radius * glm::sin(stackAngle); // sin

        for (int j = 0; j <= sectorCount; j++)
        {
            sectorAngle = j * sectorStep;

            //Vertex pos
            x = xy * glm::cos(sectorAngle);
            y = xy * glm::sin(sectorAngle); 

            //normal pos
            nx = x * invertLength;
            ny = y * invertLength;
            nz = z * invertLength;

            //tex pos
            s = (float)j / sectorCount;   
            t = (float)i / stackCount;  

            vertices.emplace_back(Vertex({ x,y,z }, { nx,ny,nz }, { s,t }));

        }
    }

    int k1, k2;

    for (int i = 0; i < stackCount; i++)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; j++, k1++, k2++)
        {
            if (i != 0)
            {
                indices.emplace_back(k1);
                indices.emplace_back(k2);
                indices.emplace_back(k1 + 1);
            }
            if (i != (stackCount - 1))
            {
                indices.emplace_back(k1 + 1);
                indices.emplace_back(k2);
                indices.emplace_back(k2 + 1);
            }
        }
    }
    return std::make_shared<T>("Sphere", vertices, indices, true, GLDrawType::Triangle);
}

template <typename T>
std::shared_ptr<T> CircleShape(const glm::vec3& origin, const float& radius, const int& num_segment)
{
    std::vector<Vertex> vertices;
    std::vector<Index> indices;

    //Define PI and Diamatre
    float PI = 3.14159265358979323846f;
    float diam = 2.0f * PI;
   
    //Set Origin points for vertex axis`s
    float x = origin.x;
    float z = origin.z; 

    //Iterate over the segments and add new vertices accordingly
    for (int i = 0; i < num_segment; i++)
    {
        vertices.emplace_back(x + (radius * glm::cos(i * diam / num_segment)), 0.f, z + (radius * glm::sin(i * diam / num_segment))); 
        indices.emplace_back(i);
    }
    return std::make_shared<T>("Circle", vertices, indices, true, GLDrawType::Triangle_Fan);  
}



//Curves and Planes ____________________________________________
template <typename T> 
std::shared_ptr<T> BSplineSurfaceShape(const int& resU, const int& resV, const int& du, const int& dv, std::vector<std::vector<glm::vec3>> controlPoints, const float& size)
{
    //Creating fundament
    std::vector<Vertex> vertices; 
    std::vector<Index> indices; 

    for (float i = 0; i < resU; i++)
    {
        auto u = i / (resU - 1); //Normalize u

        for (float j = 0; j < resV; j++)
        {
            auto v = j / (resV - 1); // Normalize v

            //Evaluate the surface in both direction U & V
            glm::vec3 surfacePoint = MathLib::EvaluateBSplineSurface(u, v, du, dv,  controlPoints);
            glm::vec3 surfaceNormal = MathLib::EvaluateBSplineSurfaceNormal(u, v, du, dv, resU, resV, controlPoints);
            glm::vec2 texCoords = glm::vec2(u, v);

            vertices.emplace_back(Vertex(surfacePoint * size, surfaceNormal, texCoords));    
        }
    }

    for (int i = 0; i < resU - 1; i++)
    {
        for (int j = 0; j < resV - 1; j++)
        {
            //Iterate over the resolution for both vectors and add them to corresponding vector
            int topLeft = i * resV + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * resV + j;
            int bottomRight = bottomLeft + 1;

            //Emplacing back indices to form the triangle, effectively creating half of a square shape
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            //Emplacing back the missing indices to shape the upper triangle, effectively creating a square
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    return std::make_shared<T>("BSplineSurface", vertices, indices, true, GLDrawType::Triangle);
}

template <typename T>
std::shared_ptr<T> InterpolationCurve3PointsShape(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3,
                                                   const float& startVal, const float& endingVal, const float& resolution)
{
    std::vector<Vertex> vertices;
    ///Creating random numbers
    srand((unsigned)time(NULL));


    //Creating an array of vec2/points
    std::vector<glm::vec2> Points
    {
        p1,
        p2,
        p3
    };

    //Creating variables for matrix A and vector b
    glm::mat3x3 A{ 0.0f };
    glm::vec3 b{ 0.0f,0.0f,0.0f };

    //Iterating over Points.size and adding respective values to respective variable
    for (auto i = 0; i < Points.size(); i++)
    {
        A[0][i] = pow(Points[i].x, 2.0f); //quadric X-term
        A[1][i] = Points[i].x;            //X-term
        A[2][i] = 1.0f;                   //Constant scalar-term

        b[i] = Points[i].y;

    }

    //GetInverse of A
    auto getInverse = glm::inverse(A);
    auto getX = getInverse * b;

    //Crating an array of Vertices
    std::vector<unsigned int> indices;

    for (float x = startVal; x <= endingVal; x += resolution)
    {
        float calcF = (getX.x * (float)pow(x, 2)) + (getX.y * x) + (getX.z);
        vertices.emplace_back(x, 0.0f, calcF);
    }

    for (int i = 0; i < vertices.size(); i++)
    {
        indices.emplace_back(i);
    }

    return std::make_shared<T>("InterpolationCurve", vertices, indices, true, GLDrawType::Line_Strip);
}

template <typename T>
std::shared_ptr<T> PlaneXZShape(const float& xMin, const float& zMin, const float& xMax, const float& zMax, const float& resolution)
{

    if ((xMin >= xMax) || (zMin >= zMax))  
    {
        std::cout << "[WARNING]:xMin or zMin is lesser than xMax or zMax \n\n";
        return nullptr;
    }
    std::cout << "[LOG]:Loading PlaneXZ \n";
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
    int i = 0;
    float nx = 0.f;
    float ny = 0.f;
    float nz = 1.f; 

    float maxVerts = (xMax - xMin) * (zMax - zMin) / (resolution * resolution);
    std::cout << "[LOG]:Amount of Vertices to Calcuate:" << maxVerts << "\n"; 

    for (auto x = xMin; x < xMax; x += resolution)
    {
        for (auto z = zMin; z < zMax; z += resolution) 
        {
            //Texture Coordinates
            float s = (x - xMin) / (xMax - xMin);
            float t = (z - zMin) / (zMax - zMin);

            float y0 = glm::cos(x) * glm::cos(z); // Bottom Left  
            float y1 = glm::cos(x + resolution) * glm::cos(z); // Bottom Right 
            float y2 = glm::cos(x) * glm::cos(z + resolution); // Top Left  
            float y3 = glm::cos(x + resolution) * glm::cos(z + resolution); // Top Right

            // Lower Triangle
            vertices.emplace_back(x, y0, z, nx, ny, nz, s, t);
            indices.emplace_back(i++); //Bottom Left

            vertices.emplace_back(x + resolution, y1, z, nx, ny, nz, s, t);
            indices.emplace_back(i++); //Bottom Right

            vertices.emplace_back(x, y2, z + resolution, nx, ny, nz, s, t);
            indices.emplace_back(i++); //Top Left

            // Upper Triangle
            vertices.emplace_back(x + resolution, y3, z + resolution, nx, ny, nz, s, t);
            indices.emplace_back(i - 2);  // Top Left   
            indices.emplace_back(i - 1); // Bottom Right 

            indices.emplace_back(i++);
        } 
    }

    auto normals = MathLib::CalculateNormalVector(vertices, indices);
    for (size_t index = 0; index < vertices.size(); index++) 
    {
        vertices[index].mNormals = normals[index];    
        //std::cout << "[LOG]:Normal Values X:" << testing[index].x << " Y:" << testing[index].y << " Z:" << testing[index].z <<"\n";    
    }
    std::cout << "[LOG]:Finished Loading\n\n";  
    return std::make_shared<T>("PlaneXZ", vertices, indices, true, GLDrawType::Triangle);
}

template <typename T>
std::shared_ptr<T> PlaneXYShape(const float& xMin, const float& yMin, const float& xMax, const float& yMax, const float& resolution)
{
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
    float z;
    int i = 0;
    float s = 0;
    float t = 0;

    for (auto x = xMin; x < xMax; x += resolution)
    {
        for (auto y = yMin; y < yMax; y += resolution)
        {
            ///Lower Triangle
            z = glm::cos(x) * glm::cos(y); //Bottom Left 
            vertices.emplace_back(x, y, z, x, y, z, s,t); 
            indices.emplace_back(i);

            z = glm::cos(x + resolution) * glm::cos(y); //Bottom Right  
            vertices.emplace_back(x + resolution, y, z, x + resolution, y, z, s, t);  
            indices.emplace_back(i + 1);

            z = glm::cos(x) * glm::cos(y + resolution); //Top Left 
            vertices.emplace_back(x, y + resolution, z, x, y + resolution, z,s,t); 
            indices.emplace_back(i + 2);

            ///Upper Triangle
            indices.emplace_back(i + 2); //Top Left 

            indices.emplace_back(i + 1); //Bottom Right

            z = glm::cos(x + resolution) * glm::cos(y + resolution); //Top Rigth
            vertices.emplace_back(x + resolution, y + resolution, z, x + resolution, y + resolution, z, s, t);  
            indices.emplace_back(i + 3);

            s = x / xMax;
            t = y / yMax; 
            i += 4; //Incrementing to next square
        }
    }

    return std::make_shared<T>("PlaneXY", vertices, indices, true, GLDrawType::Triangle);

}

template <typename T>
std::shared_ptr<T> TriangualtionGridFromLas(const char* _fileDirectory, const float& _scaleFactor, const int& _resolution) 
{
    std::cout << "[LOG]:Loading Triangulation Grid \n";
    //Loading pointcloud data from LAS
    auto data = MathLib::LoadLAS_Data(_fileDirectory, _scaleFactor);
    std::vector<Vertex> vertices = data.first;
    std::vector<Index> indices = data.second;

    //Defining local variables
    std::vector<Vertex> newVertices;
    std::vector<Index> newIndices;

    //Initialising min & max value for first vertex
    float MaxX = vertices[0].mPos.x;
    float MinX = vertices[0].mPos.x;
    float MaxZ = vertices[0].mPos.z;
    float MinZ = vertices[0].mPos.z;


    //Finding minimum and maximum values 
    for (const auto& vertex : vertices)
    {
        MaxX = glm::max(MaxX, vertex.mPos.x); 
        MinX = glm::min(MinX, vertex.mPos.x);
        MaxZ = glm::max(MaxZ, vertex.mPos.z);
        MinZ = glm::min(MinZ, vertex.mPos.z);
    }

    //Distance
    float distX = (MaxX - MinX)/ (float)_resolution;
    float distZ = (MaxZ - MinZ)/ (float)_resolution;

    //Creating spatialGrid
    std::unordered_map<std::pair<int, int>, std::vector<Vertex>, MathLib::PairHash<int,int>> spatialGrid;      

    // Populate the spatial grid
    for (const auto& vertex : vertices)
    {
        int gridX = static_cast<int>((vertex.mPos.x - MinX) / distX);
        int gridZ = static_cast<int>((vertex.mPos.z - MinZ) / distZ);

        spatialGrid[{gridX, gridZ}].push_back(vertex);
    }

    //Max num vertices
    int maxVerts = (_resolution + 1) * (_resolution + 1); 
    std::cout << "[LOG]:Amount of Vertices to Calcuate:"  << maxVerts << "\n";

    //Loading variables
    float LoadPercentage = 0;
    int totalIterations = (_resolution + 1) * (_resolution + 1);

    //Generating vertices for triangulation
    for (int i = 0; i <= _resolution; i++)
    {
        for (int j = 0; j <= _resolution; j++)       
        {
            float height = 0.f;
            int increment = 0;

            //Texture coordinates
            float s = static_cast<float> (i) / (_resolution); 
            float t = static_cast<float> (j) / (_resolution);

             glm::vec3 center = glm::vec3(MinX + distX * i, 0.f, MinZ + distZ * j); 

             //Grid indexes
             int gridX = i;
             int gridZ = j;

             //If the grid at the givem row & column is not the last pair it continues iteration
             if (spatialGrid.find({ gridX,gridZ }) != spatialGrid.end())
             {
                 //Calculatin average height based on existing vertices in partition
                 for (const auto& vertex : spatialGrid.find({ gridX,gridZ })->second)  
                 {
                     //Collision detection for height
                     if (Collision::AABBConvex(vertex.mPos, center, 200.f, 2.0f))
                     {
                         height += vertex.mPos.y;
                         increment++;
                     }
                 }
                 LoadPercentage = (static_cast<float>(i * (_resolution + 1) + j) / totalIterations) * 100;  
             }

            //Determine height of current vertex
            height = (increment > 0) ? (height / increment) : 0.f;   
            glm::vec3 vert = glm::vec3(MinX + distX * i, height, MinZ + distZ * j);     

            newVertices.emplace_back(vert.x, vert.y, vert.z, 0.0f, height, 0.0f, s ,t);   
        }
        std::cout << "[LOG]:Row:" << i << " done | Total-->Percentage:(" << LoadPercentage << "%)\n";  
        std::cout.flush();
    } 

    //Generating indices for triangulation
    for (int i = 0; i < _resolution; i++)
    {
        for (int j = 0; j < _resolution; j++)
        {
            int bottomLeft  = (i * (_resolution + 1)) + j;
            int bottomRight = bottomLeft + 1; 
            int topLeft     = ((i + 1) * (_resolution + 1)) + j;
            int topRight    = topLeft + 1; 

            //Lower Triangle
            newIndices.emplace_back(bottomLeft);
            newIndices.emplace_back(bottomRight);
            newIndices.emplace_back(topLeft); 

            //Upper
            newIndices.emplace_back(topLeft);
            newIndices.emplace_back(bottomRight);
            newIndices.emplace_back(topRight);  
        }
    }

    //Calculate normals
    auto normals = MathLib::CalculateNormalVector(newVertices, newIndices);
    for (size_t index = 0; index < newVertices.size(); index++) 
    {
        newVertices[index].mNormals = normals[index];  
        //std::cout << "[LOG]:Normal Values X:" << normals[index].x << " Y:" << normals[index].y << " Z:" << normals[index].z <<"\n";
    } 

    return std::make_shared<T>("TriangualtionGrid", newVertices, newIndices, true, GLDrawType::Triangle);
}

template <typename T>
std::pair <std::vector<Vertex>, std::vector<Index>> GetTriangualtionGridDataFromLas(const char* _fileDirectory, float _scaleFactor, float _resolution)
{
    auto data = MathLib::LoadLAS_Data(_fileDirectory, _scaleFactor);
    std::vector<Vertex> vertices = data.first;
    std::vector<Index> indices = data.second;

    std::vector<Vertex> newVertices;
    std::vector<Index> newIndices;

    float MaxX = vertices[0].mPos.x;
    float MinX = vertices[0].mPos.x;
    float MaxZ = vertices[0].mPos.z;
    float MinZ = vertices[0].mPos.z;

    //Finding minimum and maximum values 
    for (const auto& vertex : vertices)
    {
        MaxX = glm::max(MaxX, vertex.mPos.x);
        MinX = glm::min(MinX, vertex.mPos.x);
        MaxZ = glm::max(MaxZ, vertex.mPos.z);
        MinZ = glm::min(MinZ, vertex.mPos.z);
    }

    int i = 0;
    int c = 0;
    float y = 0.f;
    for (float x = MinX; x < MaxX; x += _resolution)
    {
        for (float z = MinZ; z < MaxZ; z += _resolution)
        {
            ///Upper Triangle
            //Bottom Left  
            newVertices.emplace_back(x, y, z, 0.f, 1.f, 0.f, x, z);
            newIndices.emplace_back(i);

            //Bottom Right 
            newVertices.emplace_back(x + _resolution, y, z, 0.f, 1.f, 0.f, x, z);
            newIndices.emplace_back(i + 1);

            //Top Left   
            newVertices.emplace_back(x, y, z + _resolution, 0.f, 1.f, 0.f, x, z);
            newIndices.emplace_back(i + 2);

            ///Lower Triangle
            //Top Left   
            newIndices.emplace_back(i + 2);
            //Bottom Right 
            newIndices.emplace_back(i + 1);
            //Top Rigth
            newVertices.emplace_back(x + _resolution, y, z + _resolution, 0.f, 1.f, 0.f, x, z);
            newIndices.emplace_back(i + 3);

            c++;
            i += 4;
        }
    }
    return std::make_pair(newVertices, indices); 
}


template <typename T>
std::shared_ptr<T> PointCloudFromLas(const char* _fileDirectory, float _scaleFactor)
{
    auto data = MathLib::LoadLAS_Data(_fileDirectory, _scaleFactor);
    return std::make_shared<T>("PointCloud", data.first, data.second, false, GLDrawType::Points);
}


static std::string hello()
{
    std::cout << "Hello\n";
}