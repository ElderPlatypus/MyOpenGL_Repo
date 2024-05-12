#include "Actor.h"

//Includes
#include <string>

///Acor constructor/destructor
Actor::Actor(const std::string& name, std::vector<Vertex>& vertices,std::vector<Index>& indices)
{
    mVertices = vertices;
    mIndices = indices;
    mName = name;
    configureMesh();
}

Actor::~Actor()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
}

///Create Meshes
Actor* Actor::Create2DTriangle()
{
    std::vector<Vertex> vertices =
    {
        // positions           // colors           //textures
       {{ 0.5f, -0.5f, 0.0f},  {0.5f, 0.0f, 0.0f}, {1.f,0.f}},  // bottom right
       {{-0.5f, -0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.f,0.f}},  // bottom left
       {{ 0.0f,  0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}, {1.f,1.f}}   // top 
    };

    std::vector<Index> indices =
    {
        0,1,2
    };

   return new Actor("2DTriangle", vertices, indices);
}


Actor* Actor::CreatePyramid()
{
    std::vector<Vertex> vertices =
    {
       // positions              // normals           //textures
       //Bottom Square Indices(0-3)
       {{ -0.5f, -0.5f, -0.5f},  {0.0f, -1.0f, 0.0f}, {0.f,1.f}},  // bottom closeLeft | Vertex(0)
       {{  0.5f, -0.5f, -0.5f},  {0.0f, -1.0f, 0.0f}, {1.f,1.f}},  // bottom closeRight | Vertex(1)
       {{  0.5f, -0.5f, 0.5f},   {0.0f, -1.0f, 0.0f}, {1.f,0.f}},  // bottom farRight | Vertex(2)
       {{ -0.5f, -0.5f, 0.5f},   {0.0f, -1.0f, 0.0f}, {0.f,0.f}},  // bottom farLeft | Vertex(3)

       //_________________________front & back-face_____________________
       //Front Triangle Indices(4-6)
       {{  0.0f,  0.5f, 0.0f},   {0.0f,  0.0f, 0.0f}, {0.5f,1.f}},  // top centre | Vertex(4)
       {{ -0.5f, -0.5f, -0.5f},  {0.0f, -1.0f, 0.0f}, {0.f,0.f}},   // bottom closeLeft | Vertex(5)
       {{  0.5f, -0.5f, -0.5f},  {0.0f, -1.0f, 0.0f}, {1.f,0.f}},   // bottom closeRight | Vertex(6)

       //Back Triangle Indices(7-9)
       {{  0.0f,  0.5f, 0.0f},  {0.0f,  0.0f, 0.0f}, {0.5f,1.f}},  // top centre | Vertex(7)
       {{ -0.5f, -0.5f, 0.5f},  {0.0f, -1.0f, 0.0f}, {0.f,0.f}},   // bottom farLeft | Vertex(9)
       {{  0.5f, -0.5f, 0.5f},  {0.0f, -1.0f, 0.0f}, {1.f,0.f}},   // bottom farRight | Vertex(8)

       //_________________________left & right-face_____________________
       //Right Triangle Indices(10-12)
       {{  0.0f,  0.5f, 0.0f},   {0.0f,  0.0f, 0.0f}, {0.5,1.f}},  // top centre | Vertex(10)
       {{ -0.5f, -0.5f, 0.5f},   {0.0f, -1.0f, 0.0f}, {1.f,0.f}},   // bottom farLeft | Vertex(11)
       {{ -0.5f, -0.5f, -0.5f},  {0.0f, -1.0f, 0.0f}, {0.f,0.f}},   // bottom closeLeft | Vertex(12)

       //Left Triangle Indices(13-15)
       {{  0.0f,  0.5f, 0.0f},   {0.0f,  0.0f, 0.0f}, {0.5f,1.f}},  // top centre | Vertex(10)
       {{  0.5f, -0.5f, 0.5f},   {0.0f, -1.0f, 0.0f}, {1.f,0.f}},   // bottom farRight | Vertex(2)
       {{  0.5f, -0.5f, -0.5f},  {0.0f, -1.0f, 0.0f}, {0.f,0.f}}    // bottom closeRight | Vertex(1)
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
    
    return new Actor("pyramid",vertices, indices);
}

Actor* Actor::CreateCube()
{
    std::vector<Vertex> vertices = {
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}}, // Top-left
        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
        // Left face
        {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
        // Right face
        {{ 0.5f, -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
        // Top face
        {{-0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}
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

    return new Actor("cube", vertices, indices);
}

Actor* Actor::CreateInterpolationCurve3Points(const double& startVal, const double& endingVal, const double& resolution)
{
    std::vector<Vertex> Vertices;
    ///Creating random numbers
    srand((unsigned)time(NULL));


    //Creating an array of vec2/points
    std::vector<glm::vec2> Points
    {
        glm::vec2(0,0),
        glm::vec2(1,1),
        glm::vec2(5,2)
    };

    //Creating variables for matrix A and vector b
    glm::mat3x3 A;
    glm::vec3 b;

    //Iterating over Points.size and adding respective values to respective variable
    for (auto i = 0; i < Points.size(); i++)
    {
        A[0][i] = pow(Points[i].x, 2);
        A[1][i] = Points[i].x;
        A[2][i] = 1.f;

        b[i] = Points[i].y;

    }

    //GetInverse of A
    auto getInverse = glm::inverse(A);
    auto getX = getInverse * b;

    //Crating a array of Vertices
    std::vector<unsigned int> Indices;

    for (double x = startVal; x <= endingVal; x += resolution)
    {
        double calcF = (getX.x * pow(x, 2)) + (getX.y * x) + (getX.z);
        Vertices.emplace_back(x, 0, calcF);
    }

    for (int i = 0; i < Vertices.size(); i++) 
    {
        Indices.emplace_back(i);
    }

    return new Actor("InterpolationCurve",Vertices, Indices);
}

Actor* Actor::CreatePlane(const double& xMin, const double& zMin, const double& xMax, const double& zMax, const double& resolution)
{
    std::vector<Vertex> vertices;
    std::vector<Index> inidces;
    double y;
    double pi = 2 * glm::acos(0.0f);

    for (auto x = zMin; x < zMax; x += resolution)
    {
        for (auto z = zMin; z < zMax; z += resolution)
        {
            ///Lower Triangle
            y = glm::cos(4 * x) * glm::cos(2 * z) + pi; //Bottom Left 
            vertices.emplace_back(x, y, z);

            y = glm::cos(4 * (x + resolution)) * glm::cos(2 * z) + pi; //Bottom Right 
            vertices.emplace_back(x + resolution, y, z);

            y = glm::cos(4 * x) * glm::cos(2 * (z + resolution)) + pi; //Top Left 
            vertices.emplace_back(x, y, z + resolution);


            ///Upper Triangle
            y = glm::cos(4 * x) * glm::cos(2 * (z + resolution)) + pi; //Top Left 
            vertices.emplace_back(x, y, z + resolution);

            y = glm::cos(4 * (x + resolution)) * glm::cos(2 * z) + pi; //Bottom Right
            vertices.emplace_back(x + resolution, y, z);

            y = glm::cos(4 * (x + resolution)) * glm::cos(2 * (z + resolution)) + pi; //Top Rigth
            vertices.emplace_back(x + resolution, y, z + resolution);
        }
    }

    for (int i = 0; i < vertices.size(); i += 3)
    {
        inidces.emplace_back(i);
        inidces.emplace_back(i + 1);
        inidces.emplace_back(i + 2);

    }

    return new Actor("pane", vertices, inidces);
}




///Configuring the mesh
void Actor::configureMesh()
{
    //// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(Index), mIndices.data(), GL_STATIC_DRAW);
    Vertex::configureVertexAttribs();  

    glBindVertexArray(0);
}

///Drawing the mesh
void Actor::drawActor(const Shader* shader, bool DrawLineTrueFalse) const
{
    glBindVertexArray(mVAO);
    if (shader && DrawLineTrueFalse == false)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); 
    }
    else if (shader && DrawLineTrueFalse == true)
    {
        glDrawElements(GL_LINE_STRIP, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
    }
    else 
    {
        assert(shader && "No shader found");
    }
    glBindVertexArray(0);
}










