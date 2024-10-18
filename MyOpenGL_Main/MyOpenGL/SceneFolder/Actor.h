#pragma once
//Includes std
#include <iostream>
#include <vector>
#include <string>
#include <random>
#ifndef M_PI
#define M_PI (constexpr)3.14159265358979323846
#endif

//Class includes
#include "Vertex.h"
#include "../SceneFolder/Mesh.h"
#include "../Definitions.h"
#include "../Shaders/Shader.h"
#include "../Utility/Transform.h"
#include "../Shaders/Shader.h"
#include "../CameraFolder/Camera.h"
#include "../Physics/RigidBody.h"
#include "../Physics/Environment.h" 
#include "../Utility/EnumArchive.h"
#include <glm/gtc/random.hpp>

class Actor : public Mesh
{
public:
    ///Constructor & Destructor
    //---------------------------------Members------------------------------------------------
    std::shared_ptr<Mesh> mMesh;
    std::string mName;
    //---------------------------------Methods------------------------------------------------
    Actor(const std::shared_ptr<Mesh>& mesh, const std::string &name);

    Actor(const std::shared_ptr<Actor>&) = delete;
    Actor& operator = (const std::shared_ptr<Actor>) = delete;
    Actor(const std::shared_ptr<Actor>&&) = delete;
    Actor& operator = (const std::shared_ptr<Actor>&&) = delete;

    Actor() = default; 
    ~Actor();

    ///Mesh Modifications
    //---------------------------------Methods------------------------------------------------
    void ExtrudeMesh(Extrude _increase_or_decrease, const float& _extrude) const; 
    int Id = 0;

    ///Physics
    //---------------------------------Methods------------------------------------------------
    std::shared_ptr<RigidBody> rigidB = std::make_shared<RigidBody>();
    bool EnablePhysics = true;
    glm::vec3 mVelocity = { 0.f,0.f,0.f };
    glm::vec3 mAcceleration = { 0.f,0.f,0.f };
    float mMaxSpeed = 20.f;
    void UpdateVelocity(float dt);

    ///Barycentric Coordinates
    //---------------------------------Members------------------------------------------------
    std::shared_ptr<Mesh> mSurfaceMesh{ nullptr };
    //---------------------------------Methods------------------------------------------------
    void SetBarySurfaceMesh(const std::shared_ptr<Mesh>& _selectSurface);
    std::shared_ptr<Actor> BarycentricCoordinates(float _dt);
    glm::vec3 CalculateBarycentricCoordinates(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3, glm::vec3 _playerPos);
    
    ///Lerp
    //---------------------------------Members------------------------------------------------
    std::shared_ptr<Mesh> mLerpMesh = nullptr;
    float deltaTime = 0.0f;
    float movementDir = 0.5f;
    float index = 0.0f;
    float lerpTime = -15.0f;
    //---------------------------------Methods------------------------------------------------
    void SetLerpMesh(const std::shared_ptr<Mesh>& selectLerpMesh); 
    glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float t);
    void AI_Path(float dt);

    ///Actor movement 
    //---------------------------------Members------------------------------------------ 
    std::shared_ptr<Camera> mCamera{ nullptr };
    bool mRelativeCameraPosition = false;  
    //---------------------------------Methods------------------------------------------ 
    void ActorMovement(Direction direction, const std::shared_ptr<Camera>& camera, float dt);

    ///Camera
    //---------------------------------Members------------------------------------------ 
    float mMovementSpeed = 15.0f;
    bool mIncreasingSpeed = false;
    inline static bool mAttachCamera;
    void SetCamera(const std::shared_ptr<Camera>& selectCamera);
    inline static bool AttachCamera() { return mAttachCamera = true; }
    inline static bool DetachCamera() { return mAttachCamera = false; }

    //---------------------------------Methods------------------------------------------ 
    void CameraStateControll(CameraState state, const std::shared_ptr<Camera>& camera, float dt) const;
    void cameraTracker(const std::shared_ptr<Camera>& camera, float dt) const ;

    ///Update Actors
    //---------------------------------Methods------------------------------------------ 
    void UpdateActors(float dt);

    ///Spawner 
    //---------------------------------Members------------------------------------------ 
    inline static std::vector<std::shared_ptr<Actor>> spawnVector;
    inline static std::shared_ptr<Actor> spawnedActor;
    //inline static int ActorType; //1 = cube, 2 = Sphere, 3 = Pyramid
    //---------------------------------Methods------------------------------------------ 
    static void Spawner(const int& _spawnAmount, const float& _distributionX, const float& _distributionZ, const int& _actorType);




};