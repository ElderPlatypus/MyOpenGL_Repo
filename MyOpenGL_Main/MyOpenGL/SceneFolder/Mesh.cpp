#include "Mesh.h"

//Includes
#include <random>
#include <string>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

///Constructor and Destructor
Mesh::Mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<Index>& indices, const bool& useTex, const bool& drawLine)
{
    mVertices = vertices;
    mIndices = indices;
    mName = name;
    mUseTex = useTex;
    mDrawLine = drawLine;
    configureMesh();

    for (auto& it : vertices)
    {
        minExtent = glm::min(minExtent, it.mPos);
        maxExtent = glm::max(maxExtent, it.mPos);
    }

    mExtent = maxExtent - minExtent;
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
}

///Create Meshes
Mesh* Mesh::Create2DTriangle(float size)
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

    return new Mesh("2DTriangle", vertices, indices, false, false);
}

Mesh* Mesh::CreatePyramid(float size)
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

    return new Mesh("pyramid", vertices, indices, true);
}

Mesh* Mesh::CreateCube(float size)
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

    return new Mesh("cube", vertices, indices, true, false);
}

Mesh* Mesh::CreateInterpolationCurve3Points(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3,
    const float& startVal, const float& endingVal, const float& resolution)
{
    std::vector<Vertex> Vertices;
    ///Creating random numbers
    srand((unsigned)time(NULL));


    //Creating an array of vec2/points
    std::vector<glm::vec2> Points
    {
        p1,
        p2,
        p3
        /* glm::vec2(0,0),
         glm::vec2(1,1),
         glm::vec2(5,2)*/
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
    std::vector<unsigned int> Indices;

    for (float x = startVal; x <= endingVal; x += resolution)
    {
        float calcF = (getX.x * (float)pow(x, 2)) + (getX.y * x) + (getX.z);
        Vertices.emplace_back(x, 0.0f, calcF);
    }

    for (int i = 0; i < Vertices.size(); i++)
    {
        Indices.emplace_back(i);
    }

    return new Mesh("InterpolationCurve", Vertices, Indices, false, true);
}

Mesh* Mesh::CreatePlaneXZ(const float& xMin, const float& zMin, const float& xMax, const float& zMax, const float& resolution)
{
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
    float y;
    int i = 0;

    for (auto x = xMin; x < xMax; x += resolution)
    {
        for (auto z = zMin; z < zMax; z += resolution)
        {

            ///Lower Triangle
            y = glm::cos(x) * glm::cos(z); //Bottom Left  
            vertices.emplace_back(x, y, z, 0.f, 1.f, 0.f, x, z);
            indices.emplace_back(i);

            y = glm::cos(x + resolution) * glm::cos(z); //Bottom Right 
            vertices.emplace_back(x + resolution, y, z, 0.f, 1.f, 0.f, x, z);
            indices.emplace_back(i + 1);

            y = glm::cos(x) * glm::cos(z + resolution); //Top Left  
            vertices.emplace_back(x, y, z + resolution, 0.f, 1.f, 0.f, x, z);
            indices.emplace_back(i + 2);

            ///Upper Triangle
            indices.emplace_back(i + 2);  //Top Left   

            indices.emplace_back(i + 1); //Bottom Right 

            y = glm::cos(x + resolution) * glm::cos(z + resolution); //Top Rigth
            vertices.emplace_back(x + resolution, y, z + resolution, 0.f, 1.f, 0.f, x, z);
            indices.emplace_back(i + 3);

            i += 4; //Inrementing by 4 to get newt square
        }
    }
    return new Mesh("planeXZ", vertices, indices, false, false);
}

Mesh* Mesh::CreatePlaneXY(const float& xMin, const float& yMin, const float& xMax, const float& yMax, const float& resolution)
{
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
    float z;
    int i = 0;

    for (auto x = xMin; x < xMax; x += resolution)
    {
        for (auto y = yMin; y < yMax; y += resolution)
        {
            ///Lower Triangle
            z = glm::cos(x) * glm::cos(y); //Bottom Left 
            vertices.emplace_back(x, y, z, 1.f, 1.f, 1.f, x, y);
            indices.emplace_back(i);

            z = glm::cos(x + resolution) * glm::cos(y); //Bottom Right  
            vertices.emplace_back(x + resolution, y, z, 1.f, 1.f, 1.f, x, y);
            indices.emplace_back(i + 1);

            z = glm::cos(x) * glm::cos(y + resolution); //Top Left 
            vertices.emplace_back(x, y + resolution, z);
            indices.emplace_back(i + 2);

            ///Upper Triangle
            indices.emplace_back(i + 2); //Top Left 

            indices.emplace_back(i + 1); //Bottom Right

            z = glm::cos(x + resolution) * glm::cos(y + resolution); //Top Rigth
            vertices.emplace_back(x + resolution, y + resolution, z, 1.f, 1.f, 1.f, x, y);
            indices.emplace_back(i + 3);

            i += 4; //Incrementing to next square
        }
    }
    return new Mesh("planeXY", vertices, indices, false, false);
}

Mesh* Mesh::CreateSphere(const int& stackCount, const int& sectorCount, const int& radius)
{
    std::vector<Vertex> vertices;
    std::vector<Index> indices;

    //uniform variables
    float x, y, z, xy; //position
    float nx, ny, nz, invertLength = 1.0f / radius; //normal
    float s, t; //textCoord

    //sphere segment logic
    float sectorStep = (float)(2 * M_PI / sectorCount); //Y->direction sub-divisons
    float stackStep = (float)(M_PI / stackCount); //x->direction sub-divisons
    float sectorAngle, stackAngle; //angle variables for y|x

    for (int i = 0; i <= stackCount; i++)
    {
        stackAngle = (float)M_PI / 2 - i * stackStep; //pi->-pi domain
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
    return new Mesh("Sphere", vertices, indices, true, false);
}

///Configuring the mesh
void Mesh::configureMesh()
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
void Mesh::drawActor(const Shader* shader) const
{
    glBindVertexArray(mVAO);
    if (shader && mDrawLine == false)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
    }
    else if (shader && mDrawLine == true)
    {
        glDrawElements(GL_LINE_STRIP, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
    }
    else
    {
        assert(shader && "No shader found");
    }
    glBindVertexArray(0);
}

