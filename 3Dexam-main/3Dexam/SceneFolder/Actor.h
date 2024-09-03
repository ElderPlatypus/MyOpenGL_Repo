#pragma once
//Includes std
#include <iostream>
#include <vector>
#include <string>


//Includes glm
#include "Vertex.h"
#include "../Definitions.h"
#include "../Shaders/Shader.h"
#include "../Utility/Transform.h"
#include "../Shaders/Shader.h"
#include "../CameraFolder/Camera.h"


class Actor 
{
public:
    ///Constructor & Destructor
    Actor(
          const std::string& name, std::vector<Vertex>& vertices,
          std::vector<Index>& indices, const bool& useTex,
          const bool& drawLine = false
         );

    Actor(const Actor&) = delete;
    Actor& operator = (const Actor&) = delete;
    Actor(const Actor&&) = delete;
    Actor& operator = (const Actor&&) = delete;

    Actor() = default; 
    ~Actor();

    ///Actor vertex data
    static Actor* Create2DTriangle();
    static Actor* CreatePyramid();
    static Actor* CreateCube();
    static Actor* CreateInterpolationCurve3Points(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3,
                                                  const float& startVal, const float& endingVal, const float& resolution);
    static Actor* CreatePlaneXZ(const float& xMin, const float& zMin, const float& xMax, const float& zMax, const float& resolution);
    static Actor* CreatePlaneXY(const float& xMin, const float& yMin, const float& xMax, const float& yMax, const float& resolution);
    static Actor* CreateSphere(const int& stackCount, const int& sectorCount, const int& radius);

    ///Barycentric Coordinates
    //---------------------------------Bary-Coords Methods------------------------------------------------
    void SetSurfaceActor(Actor* selectSurface);
    Actor* BarycentricCoordinates(Actor* surface,float dt);
    glm::vec3 CalculateBarycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,  glm::vec3 playerPos); 
    
    //---------------------------------Bary Coords-members------------------------------------------------
    Actor* confirmSurface = nullptr;


    ///Configure and Draw
    void configureMesh(); //Binds VAO,VB & EBO to respective mesh
    void drawActor(const Shader* shader)const ; 

    ///Transformation
    //---------------------------------Local Setters------------------------------------------------
    void SetTransformation(const Transform& transform) { mTransform = transform; }
    void SetLocalPosition(const glm::vec3& position) { mTransform.SetPosition(position); }
    void SetLocalRotation(const glm::quat& rotation) { mTransform.SetRotation(rotation); }
    void SetLocalScale(const glm::vec3& scale) {  mTransform.SetScale(scale);}
    void SetLocalTransformMatrix(const glm::mat4& transformMatrix) { mTransform.SetTransformMatrix(transformMatrix); }
  
    //---------------------------------Local Getters------------------------------------------
    const Transform& GetTransform() const { return mTransform; }
    const glm::vec3& GetLocalPosition() const { return mTransform.GetPosition(); }
    const glm::quat& GetLocalRotation() const { return mTransform.GetOrientation(); }
    const glm::vec3& GetLocalScale() const { return mTransform.GetScale(); }
    const glm::mat4 GetLocalTransformMatrix() const { return mTransform.GetTransformMatrix(); }
    glm::vec3 GetRightVector() const { return mTransform.GetRightVector(); }
   
    ///Textures
    void SetShader(Shader* shader) { mShader = shader ; }
    void UseTexture(const bool& useBool) 
    {
        mUseTex = useBool;
        mShader->setBool("useTex", useBool);  
    } 
    const bool& GetTexBool() const { return mUseTex; }

    ///Lighting
    float mAmbientStrength = 1.0f;
    glm::vec3 mLightColor{ 1.f, 1.f, 1.f };
    glm::vec3 mLightPos{ 0.f,0.f,0.f };
    glm::vec3 mObjectColor{ 1.f,0.7f,0.2f };
    bool mUseLight = true;
    void UseLight(const bool& useBool)
    {
        mUseLight = useBool;
        if (mUseLight == true)
        {
            mShader->use();
            mShader->setVec3("lightColor", mLightColor);
            mShader->setVec3("lightPos", mLightPos);
            mShader->setVec3("objectColor", mObjectColor);
        }
        else
        {
            mShader->use();
            mShader->setVec3("lightColor", glm::vec3(0.f));
            mShader->setVec3("lightPos", glm::vec3(0.f));
            mShader->setVec3("objectColor", glm::vec3(0.f));
        }
    }
    const bool& GetLightBool() const { return mUseLight; }


    ///Members and Variables
    //---------------------------------Struct and Transform------------------------------------------ 
    std::vector<Vertex> mVertices{};
    std::vector<Index> mIndices{};
    std::string mName{};
    VAO mVAO{ 0U }; 
    VBO mVBO{ 0U }; 
    EBO mEBO{ 0U };
    Transform mTransform{};

    //---------------------------------Draw and texture------------------------------------------
    bool mUseTex = false;
    bool mDrawLine = false; 
    Shader* mShader{ nullptr };


    ///Actor movement 
    //---------------------------------Actor MOvement------------------------------------------ 
    Camera* camera{ nullptr };
    bool mRelativeCameraPosition = false;  
    void ActorMovement(Direction direction, Camera* camera, float dt); 

    //---------------------------------Camera------------------------------------------ 
    void CameraControll(Direction placement, Camera* camera, float dt) const;
    float mMovementSpeed = 5.0f;
    bool mIsMoving = false;
    inline static bool mAttachCamera;


    ///Collision 
    //---------------------------------Vectors Bounding Box------------------------------------------ 
    glm::vec3 mCenter{ 0.f,0.f,0.f };
    glm::vec3 mExtent{ 0.5f,0.5f,0.5f };

    //---------------------------------Booleans------------------------------------------ 
    bool mEnableCollison = false;
    bool mIsColliding = false; 


    ///Update Actors
    void UpdateActors(float dt);


    ///Spawner 
    //---------------------------------Spawner Vector & Amount------------------------------------------ 
    inline static std::vector<Actor*> spawnVector;    
    inline static Actor* spawnedActor;
     
    //---------------------------------Spawner function------------------------------------------ 
    inline static int ActorType; //1 = cube, 2 = Sphere, 3 = Pyramid
   
    static void Spawner(const int& spawnAmount);  

    ///Lerp
    void ActorWalking(Actor* lerpActor,float dt);
    void SetLerpActor(Actor* selectLerpActor);
    Actor* confirmLerp = nullptr;
    glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float t);
    float deltaTime = 0.0f;
    float movementDir = 0.5f;
    float index = 0.0f;
    float lerpTime = -15.0f;
};