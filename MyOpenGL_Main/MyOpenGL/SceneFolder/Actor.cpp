#include "Actor.h"


///Acor constructor/destructor
Actor::Actor(Mesh* mesh, const std::string& name)
{
    mMesh = mesh;
    mName = name;
    EnablePhysics = true;
    rigidB.pos = mMesh->GetLocalPosition(); 
    rigidB.acceleration = Environment::gravitationalAcceleraton; 
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

void Actor::ExtrudeMesh(Extrude increase_or_decrease, const float &extrude)
{
    if (extrude == 1.0f)
    {
        std::cout << "Value has no influence. Try a value < or > than 1.0f \n";
    }
    switch (increase_or_decrease)
    {
    case increase:
        {
         for (auto& getVert : mMesh->mVertices)
         {
             getVert.mPos *= extrude; 
             mMesh->minExtent = glm::min(mMesh->minExtent, getVert.mPos); 
             mMesh->maxExtent = glm::max(mMesh->maxExtent, getVert.mPos);
         }
         mMesh->mExtent = (mMesh->maxExtent - mMesh->minExtent) * 0.5f;
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
    auto &getCamera = camera;
    if (!getCamera->mUseCameraMovement && !mAttachCamera)
    {
        glm::vec3 currentPosition = mMesh->GetLocalPosition();
        glm::vec3 movement(0.f);

        switch (direction)
        {
            //Forward & Backwards
        case Forward:
            movement.z -= mMovementSpeed * dt; break;
        case Backwards:
            movement.z += mMovementSpeed * dt; break;

            //Left & Right
        case Left:
            movement.x -= mMovementSpeed * dt; break;
        case Right:
            movement.x += mMovementSpeed * dt; break;

            //Up & Down
        case Up:
            movement.y += mMovementSpeed * dt; break;
        case Down:
            movement.y -= mMovementSpeed * dt; break;

            //Increase Speed
        case IncreaseSpeed:
            break;

        default:
            mMovementSpeed = 15.0f;
            break;
        }
        mMesh->SetLocalPosition(currentPosition + movement);
    }
    if (!getCamera->mUseCameraMovement && mAttachCamera)
    {
        glm::vec3 currentPosition = mMesh->GetLocalPosition();
        glm::vec3 movement(0.f);

        switch (direction)
        {
            //Forward & Backwards
        case Forward:
            movement.z -= mMovementSpeed * dt; break;
        case Backwards:
            movement.z += mMovementSpeed * dt; break;

            //Left & Right
        case Left:
            movement.x -= mMovementSpeed * dt; break;
        case Right:
            movement.x += mMovementSpeed * dt; break;

            //Up & Down
        case Up:
            movement.y += mMovementSpeed * dt; break;
        case Down:
            movement.y -= mMovementSpeed * dt; break;

            //Increase Speed
        case IncreaseSpeed:
            break;

        default:
            mMovementSpeed = 15.0f;
            break;
        }
        mMesh->SetLocalPosition(currentPosition + movement);
        camera->SetLocalPosition(currentPosition + glm::vec3(0.f, mMesh->mExtent.y, mMesh->mExtent.z + 20.f));
    }
}

void Actor::CameraControll(Direction placement, Camera* camera, float dt) const
{
    auto &getCamera = camera; 
    switch (placement)
    {
      case CameraFreeMovment_1:
            mAttachCamera = false;
            getCamera->mUseCameraMovement = true;
            break;

      case CameraStatic_CharacterMovement_2:
            mAttachCamera = false;
            getCamera->mUseCameraMovement = false;
            break;

      case CameraStatic_FollowPlayer_3:  
            mAttachCamera = true; 
            getCamera->mUseCameraMovement = false;
            break;
      default:
          mAttachCamera = false;
          getCamera->mUseCameraMovement = true;
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
    //if (!camera->mUseCameraMovement && !camera->mRightMouseButtonPressed)
    //{
    //    float xOffset = mMesh->GetLocalPosition().x - camera->GetLocalPosition().x;
    //    float yOffset = mMesh->GetLocalPosition().y - camera->GetLocalPosition().y;


    //    float yawRadians = glm::radians(xOffset);
    //    float pitchRadians = glm::radians(yOffset);


    //    //Creating a quaternion from angle and a normalized axis: projecting the normalized axis with the given angel whic is now the rotation domain for the quaternion.
    //    //Can bed visualize as a  cone where the flat surface is the where the quaternions merges from.
    //    glm::quat currentOrientation = mMesh->GetLocalRotation(); 
    //    glm::quat yawRotation = glm::angleAxis(yawRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //normal axis is not 0 for y-axis --> gets yaw
    //    glm::quat pitchRotation = glm::angleAxis(pitchRadians, glm::vec3(1.0f, 0.0f, 0.0f));  //normal axis is not 0 for x-axis --> gets pitch
    //    glm::quat newOrientation = glm::inverse(yawRotation) * currentOrientation * pitchRotation;
    //    newOrientation = glm::normalize(newOrientation); 
    //   
    //    camera->SetLocalRotation(newOrientation);
    //}
}

///Spawner
void Actor::Spawner(const int& spawnAmount, const float& distributionX, const float& distributionZ)
{
    for (int amount = 0; amount < spawnAmount; amount++)
    {
        if (spawnAmount > 0)
        {
            switch (ActorType)
            {
            case 1:
                spawnedActor = new Actor(Mesh::CreateCube(1.0f),"spawnedCube " + std::to_string(amount));
                spawnedActor->mMesh->mUseTex = true;
                spawnedActor->mMesh->mEnableCollision = true;
                break;
            case 2:
                spawnedActor = new Actor(Mesh::CreateSphere(16, 16, 1), "spawnedSphere " + std::to_string(amount));
                spawnedActor->mMesh->mUseTex = true;
                spawnedActor->mMesh->mEnableCollision = true;
                break;
            case 3:
                spawnedActor = new Actor(Mesh::CreatePyramid(5.0f), "spawnedPyramid " + std::to_string(amount));
                spawnedActor->mMesh->mUseTex = true;
                spawnedActor->mMesh->mEnableCollision = true;
                break;
            }
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> radiusXZ(distributionX, distributionZ);
            glm::vec3 spawnPos{ radiusXZ(gen),0.f,radiusXZ(gen) };
            spawnedActor->mMesh->SetLocalPosition(spawnPos);
            spawnedActor->rigidB.pos = spawnedActor->mMesh->GetLocalPosition();
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
    auto& getMesh = mMesh;
    //Updating the rigid body
    if (getMesh) 
    {
        rigidB.Update(dt);
        //Updaing the center value
        mMesh->mCenter = mMesh->GetLocalPosition();

        if (EnablePhysics)
        {
            mMesh->SetLocalPosition(rigidB.pos);   
        }
        //Bind shaders to Model-uniform
        mMesh->mShader->setMat4("model", mMesh->GetLocalTransformMatrix()); 
        mMesh->UseTexture(mMesh->GetTexBool());
        mMesh->UseLight(mMesh->GetLightBool());
        mMesh->drawActor(mMesh->mShader);

        //Confirms if any calculation bools  are enabled
        if (mSurfaceMesh)
        {
            BarycentricCoordinates(dt);
        }
        if (mLerpMesh)
        {
            AI_Path(dt);
        }
    }
    else 
    { 
        assert(mMesh && "No Mesh found"); 
        return; 
    }
}









