#include "Actor.h"

///Acor constructor/destructor
Actor::Actor(const std::shared_ptr<Mesh>& mesh, const std::string& name): mMesh(mesh), mName(name)
{
    mId++;
    mEnablePhysics = false;
    rigidB->pos = mMesh->GetLocalPosition(); 
    rigidB->acceleration = Environment::gravitationalAcceleraton; 
}

Actor::~Actor()
{
    if (!mMesh) return;  

    mMesh->~Mesh();
    mMesh = nullptr; 
}

void Actor::ExtrudeMesh(Extrude _increase_or_decrease, const float& _extrude) const
{
    if (_extrude == 1.0f)
    {
        std::cout << "Value has no influence. Try a value < or > than 1.0f \n";
    }
    switch (_increase_or_decrease)
    {
    case Increase:
        {
         for (auto& getVert : mMesh->mVertices)
         {
             getVert.mPos *= _extrude;
             mMesh->minExtent = glm::min(mMesh->minExtent, getVert.mPos); 
             mMesh->maxExtent = glm::max(mMesh->maxExtent, getVert.mPos);
         }
         mMesh->mExtent = (mMesh->maxExtent - mMesh->minExtent) * 0.5f;
         mMesh->updateVertexAttribs();

         break;
        }
    case Decrease: 
       {
        for (auto& getVert: mMesh->mVertices)
        {
            getVert.mPos /= _extrude;
        }
        mMesh->updateVertexAttribs();
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

void Actor::EnablePhysics(const bool& enablePhysics)
{
    if (!mMesh)
    {
        std::cerr << "[WARNING]:No Mesh detected \n";
        return;
    }
    else
    {
        mEnablePhysics = enablePhysics; 
        //std::cout << "[LOG]:Actor Physics Updated Successfully \n";
    }
    //std::cout << "\n";
}


///Barycentric Coordinates
void Actor::SetBarySurfaceMesh(const std::shared_ptr<Mesh>& _selectSurface)
{
    if (!_selectSurface) 
    {
        assert(_selectSurface && "Illegal surface detected or not set correctly\n");
        return;
    }
    mSurfaceMesh = _selectSurface; 
    //std::cout << "[LOG]:Actor BarySurface Updated Successfully \n";
    //std::cout << "\n";
    return;
}

void Actor::UpdateBarycentricCoords(float dt) const 
{
    MathLib::DoBarycentricCoordinatesMesh<Mesh,Mesh>(mMesh, mSurfaceMesh, dt);  
}

void Actor::UpdateBarycentricCoordsRigidB(const std::shared_ptr<Actor>& actor, float dt) const 
{ 
    MathLib::DoBarycentricCoordinatesActor<Actor,Mesh>(actor, mSurfaceMesh, dt);    
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
            mMovementSpeed = mMovementSpeed; 
            break;

        default:
            mMovementSpeed = mMovementSpeed;
            break; 
        }
        
        SetLocalRotation(camera->GetLocalRotation());    
        SetLocalPosition(GetLocalPosition() + movement);
        camera->SetMaxMovementSpeed(mMovementSpeed);
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

///Update Actors
void Actor::UpdateActors(float dt)
{
    if (!mMesh){ assert(mMesh && "No Mesh found\n"); return;}

    //Updating the rigid body
    if (mMesh) 
    {

        if (mEnablePhysics)
        {
            mMesh->SetLocalPosition(rigidB->pos); 
            UpdateVelocity(dt); 
            rigidB->Update(dt);
            mMesh->mCenter = GetLocalPosition();
        }
        else
        {
            //Updaing the center value
            mMesh->mCenter = GetLocalPosition();
        }
       
        //Bind shaders to Model-uniform
        mMesh->mShader->setMat4("model", mMesh->GetLocalTransformMatrix()); 

        //Configure texture
        mMesh->TexConfig(GetTexBool(), GetTexType());  

        //Configure light
        mMesh->LightConfig(GetLightBool(), GetLightType()); 

        //Draw actor`s mesh
        mMesh->drawActor(mMesh->mShader);


        //Additional updtes
      /*  if (mSurfaceMesh) 
        {
            UpdateBarycentricCoords(dt);
        }*/
        if (mLerpMesh)
        { 
            AI_Path(dt);
        }
    }
}

