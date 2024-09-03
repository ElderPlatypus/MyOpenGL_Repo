#include "Actor.h"

//Includes
#include <random>
#include <string>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

///Acor constructor/destructor
Actor::Actor(const std::string& name, std::vector<Vertex>& vertices,std::vector<Index>& indices, const bool& useTex, const bool& drawLine)
{
    mVertices = vertices;
    mIndices = indices;
    mName = name;
    mUseTex = useTex;
    mDrawLine = drawLine;
    mEnableCollison = false;
    mIsColliding = false;
    mAttachCamera = false;
    mUseLight = true;
    configureMesh();


    //Offset for Boudning Box
    glm::vec3 minExtent{ 0.f, 0.f, 0.f };
    glm::vec3 maxExtent{ 0.f, 0.f, 0.f };

    for (auto &it : vertices)
    {
        minExtent = glm::min(minExtent, it.mPos);
        maxExtent = glm::max(maxExtent, it.mPos);
    }

    mExtent = maxExtent - minExtent;
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

   return new Actor("2DTriangle", vertices, indices, false, false);
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
    
    return new Actor("pyramid",vertices, indices, true); 
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
        {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f, 0.0f}, {0.0f, 1.0f}},
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

    return new Actor("cube", vertices, indices, true, false);
}

Actor* Actor::CreateInterpolationCurve3Points(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3,
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
    glm::mat3x3 A{0.0f};
    glm::vec3 b{0.0f,0.0f,0.0f};

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

    return new Actor("InterpolationCurve",Vertices, Indices, false, true);
}

Actor* Actor::CreatePlaneXZ(const float& xMin, const float& zMin, const float& xMax, const float& zMax, const float& resolution)
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
    return new Actor("planeXZ", vertices, indices, false, false);
}

Actor* Actor::CreatePlaneXY(const float& xMin, const float& yMin, const float& xMax, const float& yMax, const float& resolution)
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
            vertices.emplace_back(x + resolution, y +resolution, z, 1.f, 1.f, 1.f, x, y); 
            indices.emplace_back(i + 3);

            i += 4; //Incrementing to next square
        }
    }
    return new Actor("planeXY", vertices, indices, false, false);
}

Actor* Actor::CreateSphere(const int& stackCount, const int& sectorCount, const int& radius)
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
        stackAngle = (float) M_PI / 2 - i * stackStep; //pi->-pi domain
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
    return new Actor("Sphere", vertices, indices, true, false);
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


///Barycentric Coordinates
void Actor::SetSurfaceActor(Actor* selectSurface)
{
    confirmSurface = selectSurface;
}

Actor* Actor::BarycentricCoordinates(Actor* surface,float dt)
{
    
    //vector of vertices and indices
    for (auto i = 0; i < surface->mIndices.size(); i += 3)
    {
        //Collect indices which creates each triangle in the plane
        unsigned int index1;
        unsigned int index2;
        unsigned int index3;

        //Assigning the values
        index1 = surface->mIndices[i];
        index2 = surface->mIndices[static_cast<std::vector<std::seed_seq::result_type, std::allocator<std::seed_seq::result_type>>::size_type>(i) + 1];
        index3 = surface->mIndices[static_cast<std::vector<std::seed_seq::result_type, std::allocator<std::seed_seq::result_type>>::size_type>(i) + 2];

        //Collecting the postions of the indices 
        glm::vec3 point1{ surface->mVertices[index1].mPos + surface->GetLocalPosition() * surface->GetLocalScale() }; 
        glm::vec3 point2{ surface->mVertices[index2].mPos + surface->GetLocalPosition() * surface->GetLocalScale() }; 
        glm::vec3 point3{ surface->mVertices[index3].mPos + surface->GetLocalPosition() * surface->GetLocalScale() };

        //Initialising variable which calculates bary-coords using the CalcBary-coords method
        glm::vec3 baryCoords = CalculateBarycentricCoordinates(point1,point2,point3, GetLocalPosition());  

        //Creating a variable which utilizes the calc-bary method.
        float baryHeight = ((baryCoords.x * point1.y) + (baryCoords.y * point2.y + (mExtent.y / 2)) + (baryCoords.z * point3.y));

        //If-checks if certain criterias are met folowing the rules for bary-coords behaviour
        if (baryCoords.x == 0 || baryCoords.y == 0 || baryCoords.z == 0)
        {
            SetLocalPosition(GetLocalPosition() + glm::vec3(0.01f, 0.f, 0.01f)); 
            baryCoords = CalculateBarycentricCoordinates(point1, point2, point3, GetLocalPosition()); 
        }

        if (    baryCoords.x > 0 
             && baryCoords.y > 0 
             && baryCoords.z > 0 )
        {
            SetLocalPosition(glm::vec3(GetLocalPosition().x, baryHeight, GetLocalPosition().z));
            //std::cout << "Bary coords works: " << std::endl;
            //std::cout << baryHeight << std::endl;
        }

    }
    return nullptr;
}

glm::vec3 Actor::CalculateBarycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,  glm::vec3 playerPos) 
{
    ///Setting default values to zero
    p1.y = 0.f;
    p2.y = 0.f;
    p3.y = 0.f;
    playerPos.y = 0.f;
    glm::vec3 baryCoords{0.f,0.f,0.f};
  
    ///Calculating triangle surface area
    glm::vec3 u = { p2 - p1}; //sw
    glm::vec3 v = { p3 - p1 }; //sw
    glm::vec3 n = glm::cross(u, v); 
    float triangleSurfaceArea = n.y;  

    ///Sub Triangle vectors
    glm::vec3 newU = { glm::cross(p2 - playerPos, p3 - playerPos) };
    glm::vec3 newV = { glm::cross(p3 - playerPos, p1 - playerPos) };
    glm::vec3 newW = { glm::cross(p1 - playerPos, p2 - playerPos) };

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


///Drawing the mesh
void Actor::drawActor(const Shader* shader) const
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


///Actor movement
void Actor::ActorMovement(Direction direction, Camera* camera, float dt)
{
  
    if (mRelativeCameraPosition == true && camera->mUseCameraMovement == false && mAttachCamera == false)   
    {
        switch (direction)
        {
            //Forward & Backwards
        case Forward:
            SetLocalPosition(GetLocalPosition() - glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt); 
            break;

        case Backwards:
            SetLocalPosition(GetLocalPosition() + glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
            break;

            //Left & Right
        case Left:
            SetLocalPosition(GetLocalPosition() - glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
            break;

        case Right:
            SetLocalPosition(GetLocalPosition() + glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
            break;

            //Up & Down
        case Up:
            SetLocalPosition(GetLocalPosition() + glm::vec3(0.f, 1.0f * mMovementSpeed, 0.f) * dt); 
            break; 

        case Down: 
            SetLocalPosition(GetLocalPosition() + glm::vec3(0.f, -1.0f * mMovementSpeed, 0.f) * dt); 
            break;

            //Increase Speed
        case IncreaseSpeed:
            mIsMoving = true;
            break; 
        }

        if (mIsMoving == true)
        {
            mMovementSpeed = 10.0f;
            mIsMoving = false;
        }
        else
        {
            mIsMoving = false;
            mMovementSpeed = 5.0f;
        }
    }

    if (mRelativeCameraPosition == true && camera->mUseCameraMovement == false && mAttachCamera == true)
    {
        switch (direction)
        {
            //Forward & Backwards
        case Forward:
            SetLocalPosition(GetLocalPosition() - glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
            break;

        case Backwards:
            SetLocalPosition(GetLocalPosition() + glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
            break;

            //Left & Right
        case Left:
            SetLocalPosition(GetLocalPosition() - glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
            break;

        case Right:
            SetLocalPosition(GetLocalPosition() + glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
            break;

            //Up & Down
        case Up:
            SetLocalPosition(GetLocalPosition() + glm::vec3(0.f, 1.0f * mMovementSpeed, 0.f) * dt);
            break;

        case Down:
            SetLocalPosition(GetLocalPosition() + glm::vec3(0.f, -1.0f * mMovementSpeed, 0.f) * dt);
            break;

            //Increase Speed
        case IncreaseSpeed:
            mIsMoving = true;
            break;
        }

        if (mIsMoving == true)
        {
            mMovementSpeed = 10.0f;
            mIsMoving = false;
        }
        else
        {
            mIsMoving = false;
            mMovementSpeed = 5.0f;
        }

        camera->SetLocalPosition(glm::vec3(
            GetLocalPosition().x,
            GetLocalPosition().y + 2.f,
            GetLocalPosition().z + 5.f));
    }

    /*  std::cout << "Speed " <<  mMovementSpeed << std::endl;
      std::cout << "IsMoving " << mIsMoving << std::endl;
      std::cout << "Check Local pos: "
      << GetLocalPosition().x << " "
      << GetLocalPosition().y << " "
      << GetLocalPosition().z << " "
      << std::endl;  */
}

void Actor::CameraControll(Direction placement, Camera* camera, float dt) const
{
    switch (placement)
    {
    case CameraFreeMovment_1:
        if (mRelativeCameraPosition == true)
        {
            mAttachCamera = false;
            camera->mUseCameraMovement = true;
        } 
        break;

    case CameraStatic_CharacterMovement_2:
        if (mRelativeCameraPosition == true)
        {
            mAttachCamera = false;
            camera->mUseCameraMovement = false;
        }
        break;

    case CameraStatic_FollowPlayer_3: 
        if (mRelativeCameraPosition == true)
        {
            mAttachCamera = true; 
            camera->mUseCameraMovement = false;
        }
        break;
    }
}


///Update Actors
void Actor::UpdateActors(float dt)
{
    mCenter = GetLocalPosition();
    if (confirmSurface)
    {
      BarycentricCoordinates(confirmSurface,dt); 
    }
    if (confirmLerp)
    {
        ActorWalking(confirmLerp, dt);
    }
}

void Actor::Spawner(const int& spawnAmount)
{
    for (int amount = 0; amount < spawnAmount; amount++) 
    {
        if (spawnAmount > 0)
        {
            switch (ActorType)
            {
            case 1:
                spawnedActor = Actor::CreateCube();
                spawnedActor->mUseTex = true;
                spawnedActor->mEnableCollison = true;
                break;
            case 2:
                spawnedActor = Actor::CreateSphere(5, 5, 1);
                spawnedActor->mUseTex = true;
                spawnedActor->mEnableCollison = true;
                break;
            case 3:
                spawnedActor = Actor::CreatePyramid();
                spawnedActor->mUseTex = true;
                spawnedActor->mEnableCollison = true;
                break;
            }
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> radiusXZ(-5.f, 5.f);
            glm::vec3 spawnPos{ radiusXZ(gen),0.f,radiusXZ(gen) };
            spawnedActor->SetLocalPosition(spawnPos);  
            Actor::spawnVector.emplace_back(spawnedActor); 
        }
        else
        {
            std::cout << "Amount is 0" << std::endl;
            return;
        }
    }
}

///Lerp and Lerp along interpolation curve
void Actor::ActorWalking(Actor* lerpActor, float dt)
{
    if (lerpActor == nullptr || lerpActor->mVertices.empty()) return;

    //Essential local variables
    glm::vec3 getPos(0.f);
    deltaTime += dt * movementDir;

    //Checking if timer is greater or equal to lerpTime
    if (deltaTime >= lerpTime) 
    {   
        deltaTime = 0.0f; //Resetting deltaTime
        index += movementDir; //Index Counter

        if (index == lerpActor->mVertices.size() - 1 || index == 0) 
        {
            movementDir *= -1; //Change direction if index criteria is met
        }
    }
    //update local variable with use of lerp-function
    getPos = Lerp(lerpActor->mVertices[(unsigned __int64)index].mPos + lerpActor->GetLocalPosition(), lerpActor->mVertices[(unsigned __int64)index + (unsigned __int64)movementDir].mPos + lerpActor->GetLocalPosition(), deltaTime);
    this->SetLocalPosition(getPos);
}

void Actor::SetLerpActor(Actor* selectLerpActor)
{
    confirmLerp = selectLerpActor;
}

glm::vec3 Actor::Lerp(glm::vec3 a, glm::vec3 b, float t)
{
    //Lerp formula
    return a + t * (b - a); 
}











