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

    Actor() = default; 
    Actor(const Actor&) = delete;
    ~Actor();

    ///Actor vertex data
    static Actor* Create2DTriangle();
    static Actor* CreatePyramid();
    static Actor* CreateCube();
    static Actor* CreateInterpolationCurve3Points(const double& startVal, const double& endingVal, const double& resolution);
    static Actor* CreatePlaneXZ(const double& xMin, const double& zMin, const double& xMax, const double& zMax, const double& resolution);
    static Actor* CreatePlaneXY(const double& xMin, const double& yMin, const double& xMax, const double& yMax, const double& resolution);

    ///Barycentric Coordinates
    void SetSurfaceActor(Actor* selectSurface);
    Actor* BarycentricCoordinates(Actor* surface, float dt);
    glm::vec3 CalculateBarycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,  glm::vec3 playerPos); 

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
  
    //---------------------------------Loacal Getters------------------------------------------
    const Transform& GetTransform() const { return mTransform; }
    const glm::vec3& GetLocalPosition() const { return mTransform.GetPosition(); }
    glm::vec3 GetLocalPosition(bool notConst)  { return mTransform.GetPosition(false); }
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

    ///Members and Variables
    //---------------------------------Struct and Transform------------------------------------------ 
    std::vector<Vertex> mVertices{};
    std::vector<Index> mIndices{};
    std::string mName{};
    VAO mVAO{ 0U }; 
    VBO mVBO{ 0U }; 
    EBO mEBO{ 0U };
    Transform mTransform{};
    Actor* confirmSurface = nullptr;

    //---------------------------------Draw and texture------------------------------------------
    bool mUseTex = false;
    bool mDrawLine = false; 
    Shader* mShader{ nullptr };


    ///Actor movement 
    //---------------------------------Camera------------------------------------------ 
    Camera* camera{ nullptr };
    bool mAttachCamera = false;  
    void ActorMovementNoCameraAttachment(Direction direction, Camera* camera, float dt); 

    //---------------------------------Speed------------------------------------------ 
    void CameraPlacement(Direction placement, Camera* camera, float dt) const;
    float mMovementSpeed = 5.0f;
    bool mIsMoving = false;


    ///Collision 
    //---------------------------------Vectors Bounding Box------------------------------------------ 
    glm::vec3 mCenter{ 0.f,0.f,0.f };
    glm::vec3 mExtent{ 0.5f,0.5f,0.5f };

    //---------------------------------Booleans------------------------------------------ 
    bool mEnableCollison = false;


    ///Update Actors
    void UpdateActors(float dt);

    ///Spawner test
    //---------------------------------Spawner Vector & Amount------------------------------------------ 
    inline static std::vector<Actor*> spawnVector;      
     
    //---------------------------------Spawner function------------------------------------------ 
    static void Spawner(int spawnAmount); 
    
    

};

