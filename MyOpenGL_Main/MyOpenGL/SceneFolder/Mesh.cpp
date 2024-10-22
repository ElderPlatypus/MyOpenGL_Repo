#include "Mesh.h"

///Constructor and Destructor
Mesh::Mesh(const std::string& type, const std::vector<Vertex>& vertices,
           const std::vector<Index>& indices, const bool& useTex,
           const int& GLdrawType) : mVertices(vertices), mIndices(indices), mType(type), drawType(GLdrawType)
{
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
std::shared_ptr<Mesh> Mesh::Create2DTriangle(float size)
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

    return localUpdate("2DTriangle", vertices, indices, false, 0); 
}

std::shared_ptr<Mesh> Mesh::CreatePyramid(float size)
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

    return localUpdate("pyramid", vertices, indices, true, 0);
}

std::shared_ptr<Mesh> Mesh::CreateCube(float size)
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

    return localUpdate("cube", vertices, indices, true, 0);
}

void Mesh::CreateCube2(std::shared_ptr<Mesh>& mesh, float size)
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

    localUpdate(mesh, "test", vertices, indices);
}

std::shared_ptr<Mesh> Mesh::CreateInterpolationCurve3Points(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3,
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

    return localUpdate("InterpolationCurve", vertices, indices, true, 0);
}

std::shared_ptr<Mesh> Mesh::CreatePlaneXZ(const float& xMin, const float& zMin, const float& xMax, const float& zMax, const float& resolution)
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

    return localUpdate("planeXZ", vertices, indices, true, 0); 
}

std::shared_ptr<Mesh> Mesh::CreatePlaneXY(const float& xMin, const float& yMin, const float& xMax, const float& yMax, const float& resolution)
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
    return localUpdate("planeXY", vertices, indices, true, 0);
}

std::shared_ptr<Mesh> Mesh::CreateSphere(const int& stackCount, const int& sectorCount, const int& radius)
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
    return std::make_shared<Mesh>("Sphere", vertices, indices, true, 0);
}

std::shared_ptr<Mesh> Mesh::CreateSplineSurface(int resU, int resV, int du, int dv, 
                                         const std::vector<float>& uKnot, const std::vector<float>& vKnot,
                                         const std::vector<std::vector<glm::vec3>>& controlPoints, const float& size)
{
    //Creating fundament
    std::vector<Vertex> vertices;
    std::vector<Index> indices;

    size_t numPoints_U = uKnot.size();
    size_t numPoints_V = vKnot.size(); 

    for (float i = 0; i < resU; i++)
    {
         auto u = i / (resU - 1);

        for (float j = 0; j < resV; j++) 
        {
             auto v = j / (resV - 1);

            //Evaluate the surface in both direction U & V
            glm::vec3 surfacePoint = EvaluateBSplineSurface(u, v, du, dv, uKnot, vKnot, controlPoints);
            glm::vec3 surfaceNormal = EvaluateBSplineSurfaceNormal(u, v, du, dv, resU, resV, uKnot, vKnot, controlPoints);
            glm::vec2 texCoords = glm::vec2(u, v);

            vertices.emplace_back(Vertex(surfacePoint*size, surfaceNormal*size, texCoords*size));
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

    return std::make_shared<Mesh>("BSplineSurface", vertices, indices, true, 0); 
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
void Mesh::drawActor(const std::shared_ptr<Shader>& shader) const 
{
    if (!shader) { assert(shader && "No shader found"); return; }
    if (shader)
    {
        glBindVertexArray(mVAO);
        switch (drawType)
        {
        case 0:
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;
        case 1:
            glDrawElements(GL_LINE_STRIP, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;
        case 2:
            glDrawElements(GL_POINT, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;
        default:
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;

        }
    }
   
    //glBindVertexArray(mVAO);
    //if (shader)
    //{
    //    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
    //}
    //else if (shader && mDrawLine)
    //{
    //    glDrawElements(GL_LINE_STRIP, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
    //}
    //else if (shader && mDrawPoints)
    //{
    //    glDrawElements(GL_POINT, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
    //}
    //else
    //{
    //    assert(shader && "No shader found");
    //}
    ////glBindVertexArray(0);
}

//std::shared_ptr<Mesh> Mesh::CreatePointCloudFromLASFileSurface(const char* _fileDirectory, float _scaleFactor)
//{
//    // create the reader
//    laszip_POINTER laszip_reader; 
//    if (laszip_create(&laszip_reader)) 
//
//    // open the reader
//    laszip_BOOL is_compressed;
//    if (laszip_open_reader(laszip_reader, _fileDirectory, &is_compressed))  
//
//    // get a pointer to the header of the reader that was just populated
//    laszip_header* header = nullptr;
//    if (laszip_get_header_pointer(laszip_reader, &header))
//
//    // get a pointer to the points that will be read
//    laszip_point* point;
//    if (laszip_get_point_pointer(laszip_reader, &point))
//
//    // how many points does the file have
//    laszip_I64 numTotalPoints = (header->number_of_point_records ? header->number_of_point_records : header->extended_number_of_point_records); 
//
//    // report how many points the file has
//    //LOG("Calculating %i points for TerrainSector", numTotalPoints);
//
//    std::vector<Vertex> vertices;
//    std::vector<Index> indices;
//
//    for (int i = 0; i < numTotalPoints; i++)
//    {
//        if (laszip_read_point(laszip_reader))
//
//        // Apply scaling when reading point coordinates
//        float x = (float)(point->X * header->x_scale_factor + header->x_offset) * _scaleFactor;
//        float y = (float)(point->Y * header->y_scale_factor + header->y_offset) * _scaleFactor;
//        float z = (float)(point->Z * header->z_scale_factor + header->z_offset) * _scaleFactor;
//
//        glm::vec3 colorf = glm::vec3(0);
//        glm::vec3 vertPos = glm::vec3(x, z, y);
//
//        vertices.emplace_back(PointCloudVertex(vertPos, colorf));
//        indices.emplace_back(i);
//    }
//
//    // Clean up the LASzip reader
//    if (laszip_close_reader(laszip_reader))
//  
//    if (laszip_destroy(laszip_reader))
//       
//
//    std::shared_ptr<Mesh> surface = std::make_shared<Mesh>("TerrainSector", std::move(vertices), std::move(indices));
//    return surface;
//}

