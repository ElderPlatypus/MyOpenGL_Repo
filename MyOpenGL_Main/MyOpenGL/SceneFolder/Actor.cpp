#include "Actor.h"

///Acor constructor/destructor
Actor::Actor(const std::shared_ptr<Mesh>& mesh, const std::string& name): mMesh(mesh), mName(name)
{
    mId++;
    EnablePhysics = false;
    rigidB->pos = mMesh->GetLocalPosition(); 
    rigidB->acceleration = Environment::gravitationalAcceleraton; 
}

Actor::~Actor()
{
    if (!mMesh) return;  

    mMesh->~Mesh();
    mMesh = nullptr; 
}

void Actor::DeleteSpawnvector_single(const std::shared_ptr<Actor>& actor)
{
    if (Actor::spawnVector.empty())
    {
        return;
    }

    // Remove the actor from the spawn vector
    auto it = std::find(Actor::spawnVector.begin(), Actor::spawnVector.end(), actor);
    if (it != Actor::spawnVector.end())
    {
        // Properly delete the associated mesh before erasing the actor
        if (it->get()->mMesh)
        {
            it->get()->mMesh.reset(); // Use smart pointer reset for proper memory management
        }
        Actor::spawnVector.erase(it);
    }
}

void Actor::DeleteSpawnvector_all(const std::vector<std::shared_ptr<Actor>>& actorVec)
{
    if (actorVec.empty()) 
    {
        return;
    }

    for (const auto& actor : actorVec) 
    { 
        // Remove the actor from the spawn vector
        auto it = std::find(Actor::projectileVector.begin(), Actor::projectileVector.end(), actor);
        if (it != Actor::projectileVector.end())
        {
            // Properly delete the associated mesh before erasing the actor
            if (it->get()->mMesh)
            {
                it->get()->mMesh.reset(); // Use smart pointer reset for proper memory management
            }
            Actor::projectileVector.erase(it);
        }
    }
    Actor::projectileVector.clear();
}

void Actor::ExtrudeMesh(Extrude _increase_or_decrease, const float& _extrude) const
{
    if (_extrude == 1.0f)
    {
        std::cout << "Value has no influence. Try a value < or > than 1.0f \n";
    }
    switch (_increase_or_decrease)
    {
    case increase:
        {
         for (auto& getVert : mMesh->mVertices)
         {
             getVert.mPos *= _extrude;
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
            getVert.mPos /= _extrude;
        }
        mMesh->configureMesh();
        break;
       }
    }
}

void Actor::UpdateVelocity(float dt)
{
    //Updating the velocity 
    mVelocity += mAcceleration * dt;

    //If the velocity exceeds the maximum speed, it is normalized: (unity vector) with
    //size of 1 and the multiplied with the max speed until criteria is not true.
    if (glm::length(mVelocity) > mMaxSpeed)
    {
        mVelocity = glm::normalize(mVelocity) * mMaxSpeed;
    }

    mAcceleration = glm::vec3(0.f);
}

///Barycentric Coordinates
void Actor::SetBarySurfaceMesh(const std::shared_ptr<Mesh>& _selectSurface)
{
    if (_selectSurface != nullptr)
    {
      mSurfaceMesh = _selectSurface;
    }
    else
    {
        assert(_selectSurface && "Illegal surface detected or not set correctly");
        return;
    }
}

std::shared_ptr<Actor> Actor::BarycentricCoordinates(float _dt)
{
    
    //vector of vertices and indices
    for (size_t i = 0; i < mSurfaceMesh->mIndices.size(); i += 3)
    {
        //Collect indices which creates each triangle in the plane
        /* std::shared_ptr<unsigned int> index1;
         std::shared_ptr<unsigned int> index2;
         std::shared_ptr<unsigned int> index3; */

        //Assigning the values
        const unsigned int index1 = mSurfaceMesh->mIndices[i];
        const unsigned int index2 = mSurfaceMesh->mIndices[i + 1];
        const unsigned int index3 = mSurfaceMesh->mIndices[i + 2];

        //Collecting the postions of the indices 
        glm::vec3 point1 = mSurfaceMesh->mVertices[index1].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale();
        glm::vec3 point2 = mSurfaceMesh->mVertices[index2].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale();
        glm::vec3 point3 = mSurfaceMesh->mVertices[index3].mPos + mSurfaceMesh->GetLocalPosition() * mSurfaceMesh->GetLocalScale();

        //Initialising variable which calculates bary-coords using the CalcBary-coords method
        glm::vec3 baryCoords = CalculateBarycentricCoordinates(point1,point2,point3, mMesh->GetLocalPosition());

        //Creating a variable which utilizes the calc-bary method
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

glm::vec3 Actor::CalculateBarycentricCoordinates(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3, glm::vec3 _playerPos)
{
    ///Setting default values to zero
    _p1.y = 0.f;
    _p2.y = 0.f;
    _p3.y = 0.f;
    _playerPos.y = 0.f;
    glm::vec3 baryCoords{0.f,0.f,0.f};
  
    ///Calculating triangle surface area
    glm::vec3 u = { _p2 - _p1}; //sw
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

///Actor movement
void Actor::ActorMovement(Direction direction, const std::shared_ptr<Camera>& camera, float dt) 
{
    if (!camera->mUseCameraMovement && !mAttachCamera)  
    {
        glm::vec3 movement{ 0,0,0 };
        const glm::vec3& front = GetForwardVector();
        const glm::vec3& right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));
        const glm::vec3& up = glm::cross(right, front);

        //Directions are inverted
        switch (direction)
        {
            //Forward & Backwards
        case Forward:
            movement += mMovementSpeed * dt * front; break;
        case Backwards:
            movement -= mMovementSpeed * dt * front; break;

            //Left & Right
        case Left:
            movement -= mMovementSpeed * dt * right; break;
        case Right:
            movement += mMovementSpeed * dt * right; break;

            //Up & Down
        case Up:
            movement += mMovementSpeed * dt * up; break;
        case Down:
            movement -= mMovementSpeed * dt * up; break;

            //Increase Speed
        case IncreaseSpeed:
            mMovementSpeed = 50.0f;
            break;
        case NormalSpeed:
            mMovementSpeed = 15.0f;
            break;

        default:
            mMovementSpeed = 15.0f;
            break;
        }
        SetLocalRotation(glm::vec3(0,0,0));
        SetLocalPosition(GetLocalPosition() + movement);
        //camera->SetLocalPosition(GetLocalPosition() - (camera->GetForwardVector() * 10.f));
    }

    if (camera->mUseCameraMovement && mAttachCamera)
    {
        glm::vec3 movement{0,0,0};
        const glm::vec3& front = GetForwardVector();
        const glm::vec3& right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));
        const glm::vec3& up = glm::cross(right, front);

        //Directions are inverted
        switch (direction)
        {
            //Forward & Backwards
        case Forward: 
            movement += mMovementSpeed * dt * front; break;  
        case Backwards:
            movement -= mMovementSpeed * dt * front; break;

            //Left & Right
        case Left:
            movement -= mMovementSpeed * dt * right; break;
        case Right:                                   
            movement += mMovementSpeed * dt * right; break; 

            //Up & Down
        case Up:
            movement += mMovementSpeed * dt * up; break;
        case Down:
            movement -= mMovementSpeed * dt * up; break;

            //Increase Speed
        case IncreaseSpeed:
            mMovementSpeed = camera->GetAccelerationSpeed() * 1.5f;
            break;
        case NormalSpeed:
            mMovementSpeed = camera->GetAccelerationSpeed(); 
            break;

        default:
            mMovementSpeed = camera->GetAccelerationSpeed(); 
            break;
        }
        
        SetLocalRotation(camera->GetLocalRotation());    
        SetLocalPosition(GetLocalPosition() + movement);
        camera->SetLocalPosition(GetLocalPosition() - (camera->GetForwardVector() * 10.f) + (camera->GetUpVector() * 5.f)); 

    }
}

inline void Actor::SetCamera(const std::shared_ptr<Camera>& selectCamera)
{
    this->mCamera = selectCamera;
}

void Actor::CameraStateControll(CameraState state, const std::shared_ptr<Camera>& camera, float dt) const
{
    switch (state)
    {
      case CameraFreeMovement_1:
            DetachCamera(); 
            camera->mUseCameraMovement = true; 
            break;

      case CameraStatic_CharacterMovement_2:
            DetachCamera();
            camera->mUseCameraMovement = false; 
            break;

      case CameraStatic_FollowPlayer_3:  
          if (mAttachToActor)
          {
              AttachCamera();
              camera->mUseCameraMovement = true;
              return;
          }
          return;

      default:
          DetachCamera();
          camera->mUseCameraMovement = true;
    }
}

///Lerp and Lerp along interpolation curve
void Actor::SetLerpMesh(const std::shared_ptr<Mesh>& selectLerpMesh)
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
    getPos = Lerp(mMesh->mVertices[(unsigned int)index].mPos + mMesh->GetLocalPosition(), mMesh->mVertices[(unsigned __int64)index + (unsigned __int64)movementDir].mPos + mMesh->GetLocalPosition(), deltaTime);
    this->mMesh->SetLocalPosition(getPos);
}

///Camera Support
void Actor::ProjectileSpawner(const std::shared_ptr<Actor>& actor,  const std::shared_ptr<Shader>& Shader, float dt) 
{
    float timer = dt;
    int index = 0; 
    if (isShooting) 
    { 
        index++; 
        projectileActor = std::make_shared<Actor>(Mesh::CreateCube(2), "spawnedSphere " + std::to_string(index)); 

        projectileActor->UseTexBool(true); 
        projectileActor->mEnableAABBCollision = true;
        projectileActor->SetLocalRotation(actor->GetForwardVector()); 
        projectileActor->SetLocalPosition(actor->GetLocalPosition() +  100.f*dt);
        Actor::projectileVector.emplace_back(projectileActor);  
    }
    if (!Actor::projectileVector.empty())
    {
        if (timer > 10.f)
        {
            for (const auto& it : Actor::projectileVector)
            {
                actor->DeleteSpawnvector_all(projectileVector);
            }
        }
        for (const auto& actor : Actor::projectileVector) { actor->SetShader(Shader); actor->UpdateActors(dt); }
    }
    isShooting = false;
}

bool Actor::Shoot(Mouse shoot, float dt) const
{
    if (!isPlayer || !shoot)  
    {
        return false;
    }
    if (shoot)
    {
        std::cout << "shoot\n";
        isShooting = true;
        return isShooting;
    }
    return false;
}

///Spawner
void Actor::Spawner(const int& _spawnAmount, const float& _distributionX, const float& _distributionZ, const int& _actorType)
{
    numSpawn = _spawnAmount;
    for (int amount = 0; amount < _spawnAmount; amount++)
    {
        if (_spawnAmount <= 0) { std::cout << "Amount is less or equal to 0" << std::endl; return; }

        Actor::spawnVector.reserve(_spawnAmount);
        switch (_actorType)
        {
        case 1:
            spawnedActor = std::make_shared<Actor>(Mesh::CreateCube(1.0f), "spawnedCube " + std::to_string(amount));
            spawnedActor->UseTexBool(true);
            break;
        case 2:
            spawnedActor = std::make_shared<Actor>(Mesh::CreateSphere(16, 16, 1), "spawnedSphere " + std::to_string(amount));
            spawnedActor->UseTexBool(true);
            break;
        case 3:
            spawnedActor = std::make_shared<Actor>(Mesh::CreatePyramid(5.0f), "spawnedPyramid " + std::to_string(amount));
            spawnedActor->UseTexBool(true);
            break;
        }

        std::random_device rd;  // Obtain a random number from hardware
        std::mt19937 eng(rd()); // Seed the generator
        std::uniform_real_distribution<float> distr(_distributionX, _distributionZ); // Define the range

        spawnedActor->SetLocalPosition(glm::vec3(distr(eng), distr(eng), distr(eng))); 
        spawnedActor->rigidB->pos = spawnedActor->GetLocalPosition();
        Actor::spawnVector.emplace_back(spawnedActor);
    }
}

///Update Actors
void Actor::UpdateActors(float dt)
{
    if (!mMesh) return; 


    //Updating the rigid body
    if (mMesh) 
    {
        UpdateVelocity(dt);
        rigidB->Update(dt);
        //Updaing the center value
        mMesh->mCenter = mMesh->GetLocalPosition();

        if (EnablePhysics)
        {
            mMesh->SetLocalPosition(rigidB->pos);   
        }
        //Bind shaders to Model-uniform
        mMesh->mShader->setMat4("model", mMesh->GetLocalTransformMatrix()); 
        mMesh->TexBool(GetTexBool());  
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


