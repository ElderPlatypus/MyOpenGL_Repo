#pragma once
//Includes std
#include <iostream>
#include <vector>
#include <string>


//Includes glm
#include "Vertex.h"
#include "../SceneFolder/Mesh.h"
#include "../Definitions.h"
#include "../Shaders/Shader.h"
#include "../Utility/Transform.h"
#include "../Shaders/Shader.h"
#include "../CameraFolder/Camera.h"

enum Extrude
{
    increase, decrease
};

class Actor 
{
public:
    ///Constructor & Destructor
    //---------------------------------Members------------------------------------------------
    Mesh* mMesh = nullptr;
    std::string mName;
    //---------------------------------Methods------------------------------------------------
    Actor(Mesh* mesh, std::string name);

    Actor(const Actor&) = delete;
    Actor& operator = (const Actor&) = delete;
    Actor(const Actor&&) = delete;
    Actor& operator = (const Actor&&) = delete;

    Actor() = default; 
    ~Actor();

    ///Mesh Modifications

    //---------------------------------Methods------------------------------------------------
    void ExtrudeMesh(Extrude increase_or_decrease, const float extrude);
    

    ///Barycentric Coordinates
    //---------------------------------Members------------------------------------------------
    Mesh* mSurfaceMesh = nullptr;
    //---------------------------------Methods------------------------------------------------
    void SetBarySurfaceMesh(Mesh* selectSurface);
    Actor* BarycentricCoordinates(float dt);
    glm::vec3 CalculateBarycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,  glm::vec3 playerPos); 
    
    ///Lerp
    //---------------------------------Members------------------------------------------------
    Mesh* mLerpMesh = nullptr;
    float deltaTime = 0.0f;
    float movementDir = 0.5f;
    float index = 0.0f;
    float lerpTime = -15.0f;
    //---------------------------------Methods------------------------------------------------
    void SetLerpMesh(Mesh* selectLerpMesh);
    glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float t);
    void AI_Path(float dt);

    ///Actor movement 
    //---------------------------------Members------------------------------------------ 
    Camera* camera{ nullptr };
    bool mRelativeCameraPosition = false;  
    //---------------------------------Methods------------------------------------------ 
    void ActorMovement(Direction direction, Camera* camera, float dt); 

    ///Camera
    //---------------------------------Members------------------------------------------ 
    float mMovementSpeed = 5.0f;
    bool mIsMoving = false;
    inline static bool mAttachCamera;
    //---------------------------------Methods------------------------------------------ 
    void CameraControll(Direction placement, Camera* camera, float dt) const;
    void cameraTracker(Camera* camera, float dt) const ;

    ///Update Actors
    //---------------------------------Methods------------------------------------------ 
    void UpdateActors(float dt);

    ///Spawner 
    //---------------------------------Members------------------------------------------ 
    inline static std::vector<Actor*> spawnVector;
    inline static Actor* spawnedActor; 
    inline static int ActorType; //1 = cube, 2 = Sphere, 3 = Pyramid
    //---------------------------------Methods------------------------------------------ 
    static void Spawner(const int& spawnAmount);

};