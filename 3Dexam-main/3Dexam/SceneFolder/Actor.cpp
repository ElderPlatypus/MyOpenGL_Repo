#include "Actor.h"

//Includes
#include <random>
#include <string>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

///Acor constructor/destructor
Actor::Actor(Mesh* mesh, std::string name)
{
    mMesh = mesh;
    mMesh->mName = name;
}

Actor::~Actor()
{
    mMesh->~Mesh();
    delete this;

    for (auto& getMeshVector : spawnVector)
    {
        getMeshVector->mMesh->~Mesh();
        delete getMeshVector; 
    }
    spawnVector.clear();
}

void Actor::ExtrudeMesh(Extrude increase_or_decrease, const float extrude)
{
    switch (increase_or_decrease)
    {
    case increase:
        {
         for (auto& getVert : mMesh->mVertices)
         {
             getVert.mPos *= extrude; 
         }
         mMesh->configureMesh();
         break;
        }
    case decrease:
       {
        for (auto& getVert: mMesh->mVertices)
        {
            getVert.mPos /= extrude; 
        }
        mMesh->configureMesh();
        break;
       }
    }
}


///Barycentric Coordinates
void Actor::SetBarySurfaceMesh(Mesh* selectSurface)
{
    if (selectSurface != nullptr)
    {
      mSurfaceMesh = selectSurface; 
    }
    else
    {
        assert(selectSurface && "Illegal surface detected or not set correctly");
        return;
    }
}

Actor* Actor::BarycentricCoordinates(float dt)
{
    
    //vector of vertices and indices
    for (auto i = 0; i < mSurfaceMesh->mIndices.size(); i += 3)
    {
        //Collect indices which creates each triangle in the plane
        unsigned int index1;
        unsigned int index2;
        unsigned int index3;

        //Assigning the values
        index1 = mSurfaceMesh->mIndices[i];
        index2 = mSurfaceMesh->mIndices[static_cast<std::vector<std::seed_seq::result_type, std::allocator<std::seed_seq::result_type>>::size_type>(i) + 1];
        index3 = mSurfaceMesh->mIndices[static_cast<std::vector<std::seed_seq::result_type, std::allocator<std::seed_seq::result_type>>::size_type>(i) + 2];

        //Collecting the postions of the indices 
        glm::vec3 point1{ mSurfaceMesh->mVertices[index1].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale() };
        glm::vec3 point2{ mSurfaceMesh->mVertices[index2].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale() };
        glm::vec3 point3{ mSurfaceMesh->mVertices[index3].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale() };

        //Initialising variable which calculates bary-coords using the CalcBary-coords method
        glm::vec3 baryCoords = CalculateBarycentricCoordinates(point1,point2,point3, mMesh->GetLocalPosition());  

        //Creating a variable which utilizes the calc-bary method.
        float baryHeight = ((baryCoords.x * point1.y) + (baryCoords.y * point2.y + (mMesh->mExtent.y / 2)) + (baryCoords.z * point3.y));

        //If-checks if certain criterias are met folowing the rules for bary-coords behaviour
        if (baryCoords.x == 0 || baryCoords.y == 0 || baryCoords.z == 0)
        {
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() + glm::vec3(0.01f, 0.f, 0.01f));
            baryCoords = CalculateBarycentricCoordinates(point1, point2, point3, mMesh->GetLocalPosition());
        }

        if (    baryCoords.x > 0 
             && baryCoords.y > 0 
             && baryCoords.z > 0 )
        {
            mMesh->SetLocalPosition(glm::vec3(mMesh->GetLocalPosition().x, baryHeight, mMesh->GetLocalPosition().z));
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

///Actor movement
void Actor::ActorMovement(Direction direction, Camera* camera, float dt)
{
  
    if (mRelativeCameraPosition == true && camera->mUseCameraMovement == false && mAttachCamera == false)   
    {
        switch (direction)
        {
            //Forward & Backwards
        case Forward:
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() - glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
            break;

        case Backwards:
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() + glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
            break;

            //Left & Right
        case Left:
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() - glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
            break;

        case Right:
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() + glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
            break;

            //Up & Down
        case Up:
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() + glm::vec3(0.f, 1.0f * mMovementSpeed, 0.f) * dt);
            break; 

        case Down: 
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() + glm::vec3(0.f, -1.0f * mMovementSpeed, 0.f) * dt);
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
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() - glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
            break;

        case Backwards:
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() + glm::vec3(0.f, 0.f, 1.5f * mMovementSpeed) * dt);
            break;

            //Left & Right
        case Left:
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() - glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
            break;

        case Right:
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() + glm::vec3(1.0f * mMovementSpeed, 0.f, 0.f) * dt);
            break;

            //Up & Down
        case Up:
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() + glm::vec3(0.f, 1.0f * mMovementSpeed, 0.f) * dt);
            break;

        case Down:
            mMesh->SetLocalPosition(mMesh->GetLocalPosition() + glm::vec3(0.f, -1.0f * mMovementSpeed, 0.f) * dt);
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
            mMesh->GetLocalPosition().x,
            mMesh->GetLocalPosition().y + 5.f,
            mMesh->GetLocalPosition().z + mMesh->mExtent.z + 5.f));
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

///Lerp and Lerp along interpolation curve
void Actor::SetLerpMesh(Mesh* selectLerpMesh)
{
    mLerpMesh = selectLerpMesh;
}

glm::vec3 Actor::Lerp(glm::vec3 a, glm::vec3 b, float t)
{
    //Lerp formula
    return a + t * (b - a); 
}

void Actor::AI_Path(float dt)
{
    if (mLerpMesh == nullptr || mLerpMesh->mVertices.empty()) return;

    //Essential local variables
    glm::vec3 getPos(0.f);
    deltaTime += dt * movementDir;

    //Checking if timer is greater or equal to lerpTime
    if (deltaTime >= lerpTime)
    {
        deltaTime = 0.0f; //Resetting deltaTime
        index += movementDir; //Index Counter

        if (index == mLerpMesh->mVertices.size() - 1 || index == 0)
        {
            movementDir *= -1; //Change direction if index criteria is met
        }
    }
    //update local variable with use of lerp-function
    getPos = Lerp(mMesh->mVertices[(unsigned __int64)index].mPos + mMesh->GetLocalPosition(), mMesh->mVertices[(unsigned __int64)index + (unsigned __int64)movementDir].mPos + mMesh->GetLocalPosition(), deltaTime);
    this->mMesh->SetLocalPosition(getPos);
}

///Camera Support
void Actor::cameraTracker(Camera* camera, float dt) const  
{
    if (mRelativeCameraPosition == true && camera->mUseCameraMovement == false && camera->mRightMouseButtonPressed == false)
    {
        float xOffset = mMesh->GetLocalPosition().x - camera->GetLocalPosition().x;
        float yOffset = mMesh->GetLocalPosition().y - camera->GetLocalPosition().y;


        float yawRadians = glm::radians(xOffset);
        float pitchRadians = glm::radians(yOffset);


        //Creating a quaternion from angle and a normalized axis: projecting the normalized axis with the given angel whic is now the rotation domain for the quaternion.
        //Can bed visualize as a  cone where the flat surface is the where the quaternions merges from.
        glm::quat currentOrientation = mMesh->GetLocalRotation(); 
        glm::quat yawRotation = glm::angleAxis(yawRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //normal axis is not 0 for y-axis --> gets yaw
        glm::quat pitchRotation = glm::angleAxis(pitchRadians, glm::vec3(1.0f, 0.0f, 0.0f));  //normal axis is not 0 for x-axis --> gets pitch
        glm::quat newOrientation = glm::inverse(yawRotation) * currentOrientation * pitchRotation;
        newOrientation = glm::normalize(newOrientation); 
       
        camera->SetLocalRotation(newOrientation);
    }
}

///Spawner
void Actor::Spawner(const int& spawnAmount)
{
    for (int amount = 0; amount < spawnAmount; amount++)
    {
        if (spawnAmount > 0)
        {
            switch (ActorType)
            {
            case 1:
                spawnedActor = new Actor(Mesh::CreateCube(5.0f),"spawnedCube");
                spawnedActor->mMesh->mUseTex = true;
                spawnedActor->mMesh->mEnableCollision = true;
                break;
            case 2:
                spawnedActor = new Actor(Mesh::CreateSphere(16, 16, 1), "spawnedSphere");
                spawnedActor->mMesh->mUseTex = true;
                spawnedActor->mMesh->mEnableCollision = true;
                break;
            case 3:
                spawnedActor = new Actor(Mesh::CreatePyramid(), "spawnedPyramid");
                spawnedActor->mMesh->mUseTex = true;
                spawnedActor->mMesh->mEnableCollision = true;
                break;
            }
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> radiusXZ(-10.f, 10.f);
            glm::vec3 spawnPos{ radiusXZ(gen),0.f,radiusXZ(gen) };
            spawnedActor->mMesh->SetLocalPosition(spawnPos);
            Actor::spawnVector.emplace_back(spawnedActor);
        }
        else
        {
            std::cout << "Amount is 0" << std::endl;
            return;
        }
    }
}

///Update Actors
void Actor::UpdateActors(float dt)
{
    mMesh->mCenter = mMesh->GetLocalPosition();
    if (mSurfaceMesh)
    {
        BarycentricCoordinates(dt);
    }
    if (mLerpMesh)
    {
        AI_Path(dt);
    }
}









