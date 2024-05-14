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

class Actor 
{
public:
    Actor(const std::string& name, std::vector<Vertex>& vertices, std::vector<Index>& indices, const bool& useTex, const bool& drawLine = false);
    Actor() = default; 
    Actor(const Actor&) = delete;
    ~Actor();

    //Actor vertex data
    static Actor* Create2DTriangle();
    static Actor* CreatePyramid();
    static Actor* CreateCube();
    static Actor* CreateInterpolationCurve3Points(const double& startVal, const double& endingVal, const double& resolution);
    static Actor* CreatePlaneXZ(const double& xMin, const double& zMin, const double& xMax, const double& zMax, const double& resolution);
    static Actor* CreatePlaneXY(const double& xMin, const double& yMin, const double& xMax, const double& yMax, const double& resolution);

    //Configure and drawing the objects
    void configureMesh();
    void drawActor(const Shader* shader)const ; 


    //Transformation
    //---------------------------------Local Setters------------------------------------------------
    void SetTransformation(const Transform& transform) { mTransform = transform; }
    void SetLocalPosition(const glm::vec3& position) { mTransform.SetPosition(position); }
    void SetLocalRotation(const glm::quat& rotation) { mTransform.SetRotation(rotation); }
    void SetLocalScale(const glm::vec3& scale) {  mTransform.SetScale(scale);}
    void SetLocalTransformMatrix(const glm::mat4& transformMatrix) { mTransform.SetTransformMatrix(transformMatrix); }
    
    void SetShader(Shader* shader) { mShader = shader ; }


    //---------------------------------Loacal Getters------------------------------------------
    const Transform& GetTransform() const { return mTransform; }
    const glm::vec3& GetLocalPosition() const { return mTransform.GetPosition(); }
    const glm::quat& GetLocalRotation() const { return mTransform.GetOrientation(); }
    const glm::vec3& GetLocalScale() const { return mTransform.GetScale(); }
    const glm::mat4 GetLocalTransformMatrix() const { return mTransform.GetTransformMatrix(); }
    glm::vec3 GetRightVector() const { return mTransform.GetRightVector(); }
   
    ///Textures
    void UseTexture(const bool& useBool) 
    {
        mUseTex = useBool;
        mShader->setBool("useTex", useBool);  
    }

    virtual void Update(float dt) {}
 
    const bool& GetTexBool() { return mUseTex; }


    ///Members and Variables
    std::vector<Vertex> mVertices{};
    std::vector<Index> mIndices{};
    std::string mName{};
    bool mUseTex = false;
    bool mDrawLine = false; 

    VAO mVAO{ 0U }; 
    VBO mVBO{ 0U };
    EBO mEBO{ 0U }; 
    Transform mTransform{};
    
    Shader* mShader{ nullptr };

};

