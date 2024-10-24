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
#include "../MathLib/Vertex.h"
#include "../SceneFolder/Mesh.h"
#include "../Definitions.h"
#include "../Shaders/Shader.h"
#include "../MathLib/Transform.h"
#include "../Shaders/Shader.h"
#include "../CameraFolder/Camera.h"
#include "../Physics/RigidBody.h"
#include "../Physics/Environment.h" 
#include "../Utility/EnumArchive.h"
#include <glm/gtc/random.hpp>

class Actor : public Mesh
{
private:
    std::shared_ptr<Mesh> mMesh; 
public:
    ///Constructor & Destructor
    //---------------------------------Members------------------------------------------------
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
    std::shared_ptr<Mesh> mSurfaceMesh;
    //---------------------------------Methods------------------------------------------------
    void SetBarySurfaceMesh(const std::shared_ptr<Mesh>& _selectSurface);
    std::shared_ptr<Actor> BarycentricCoordinates(float _dt);
    glm::vec3 CalculateBarycentricCoordinates(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3, glm::vec3 _playerPos);
    
    ///Lerp
    //---------------------------------Members------------------------------------------------
    std::shared_ptr<Mesh> mLerpMesh;
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
    std::shared_ptr<Camera> mCamera;
    bool mCanMove = false;
    //---------------------------------Methods------------------------------------------ 
    void ActorMovement(Direction direction, const std::shared_ptr<Camera>& camera, float dt);

    ///Camera
    //---------------------------------Members------------------------------------------ 
    float mMovementSpeed = 15.0f;
    bool mIncreasingSpeed = false;
    inline static bool mAttachCamera;
    bool mAttachToActor = false;
    void SetCamera(const std::shared_ptr<Camera>& selectCamera);
    inline static bool AttachCamera() { return mAttachCamera = true; }
    inline static bool DetachCamera() { return mAttachCamera = false; }

    //---------------------------------Methods------------------------------------------ 
    void CameraStateControll(CameraState state, const std::shared_ptr<Camera>& camera, float dt) const;

    ///Update Actors
    //---------------------------------Methods------------------------------------------ 
    void UpdateActors(float dt);

    ///Spawner 
    //---------------------------------Members------------------------------------------ 
    inline static std::vector<std::shared_ptr<Actor>> spawnVector;
    inline static std::shared_ptr<Actor> spawnedActor;
    void DeleteSpawnvector_single(const std::shared_ptr<Actor>& actor);
    void DeleteSpawnvector_all(const std::vector<std::shared_ptr<Actor>>& actorVec);
    //inline static int ActorType; //1 = cube, 2 = Sphere, 3 = Pyramid
    
    static void ProjectileSpawner(const std::shared_ptr<Actor>& actor, const std::shared_ptr<Shader>& Shader, float dt);
    bool Shoot(Mouse shoot, const std::shared_ptr<Actor>& actor, float dt) const; 
    inline static std::vector<std::shared_ptr<Actor>> projectileVector;
    inline static std::shared_ptr<Actor> projectileActor;
    inline static bool isShooting;

    //---------------------------------Methods------------------------------------------ 
    static void Spawner(const int& _spawnAmount, const float& _distributionX, const float& _distributionZ, const int& _actorType);
    inline static int numSpawn;

    ///Transform Getter & Setters
    //---------------------------------Methods Setters------------------------------------------------ 
    void SetLocalPosition(const glm::vec3& position) const { mMesh->SetLocalPosition(position); } 
    void SetLocalRotation(const glm::quat& rotation) const { mMesh->SetLocalRotation(rotation); } 
    void SetLocalScale(const glm::vec3& scale) const { mMesh->SetLocalScale(scale); }
    void SetLocalTransformMatrix(const glm::mat4& transformMatrix) const { mMesh->SetLocalTransformMatrix(transformMatrix); }
    void SetShader(const std::shared_ptr<Shader>& shader) const { mMesh->SetShader(shader); }
    const bool UseTexBool(const bool& useTex) const { return mMesh->mUseTex = useTex; } 

    //---------------------------------Methods Getters------------------------------------------
    const std::shared_ptr<Transform> GetTransform() const { return mMesh->mTransform; }
    const glm::vec3& GetLocalPosition() const { return mMesh->GetLocalPosition(); }
    const glm::quat& GetLocalRotation() const { return mMesh->GetLocalRotation(); }
    const glm::vec3& GetLocalScale() const { return mMesh->GetLocalScale(); }

    glm::mat4 GetLocalTransformMatrix() const { return mMesh->GetLocalTransformMatrix(); }
    const glm::vec3 GetForwardVector() const { return mMesh->GetForwardVector(); }
    const glm::vec3 GetRightVector() const { return mMesh->GetRightVector(); }
    const glm::vec3 GetUpVector() const { return mMesh->GetUpVector(); }
    float GetPitch() const { return mMesh->GetPitch(); }
    float GetYaw() const { return mMesh->GetYaw(); }
    void  GetdrawActor(const std::shared_ptr<Shader>& shader) const { return mMesh->drawActor(shader); }
    const bool GetTexBool() const { return mMesh->mUseTex; }


    const glm::vec3& GetExtent() const { return mMesh->mExtent; }
    const glm::vec3& GetCenter() const { return mMesh->mCenter; }

    ///Collision
    bool mEnableAABBCollision = false;
	bool mEnableInverseAABB = false;
    inline static int Health = 100; 

    int GetHealth() const { return Health; }
    bool die = false;
    bool Restart() { return die; };
};