#pragma once
//Includes std
#include <iostream>
#include <vector>

//Includes glm
#include "Vertex.h"
#include "../Definitions.h"
#include "../Shaders/Shader.h"
#include "../Utility/Transform.h"
#include "../Shaders/Shader.h"

class Actor 
{
public:
    Actor(std::vector<Vertex>& vertices, std::vector<Index>& indices);
    ~Actor();

    //Actor vertex data
    static Actor* Create2DTriangle();
    static Actor* CreatePyramid();
    static Actor* CreateCube();
    static Actor* CreateInterpolationCurve3Points(const double& startVal, const double& endingVal, const double& resolution);
    static Actor* CreatePlane(const double& xMin, const double& zMin, const double& xMax, const double& zMax, const double& resolution);
     
    //Configure and drawing the objects
    void configureMesh();
    void drawActor(const Shader* shader)const ; 


    //Transformation
    //---------------------------------Setters------------------------------------------------
    void SetTransformation(const Transform& transform) { mTransform = transform; }
    void SetLocalPosition(const glm::vec3& position) { mTransform.SetPosition(position); }
    void SetLocalRotation(const glm::quat& rotation) { return mTransform.SetRotation(rotation); }
    void SetLocalScale(const glm::vec3& scale) { return mTransform.SetScale(scale);}
    void SetLocalTransformMatrix(const glm::mat4& transformMatrix) { mTransform.SetTransformMatrix(transformMatrix); }
    
    //---------------------------------Getters------------------------------------------
    const Transform& GetTransform() { return mTransform; }
    const glm::vec3& GetLocalPosition() { return mTransform.GetPosition(); }
    const glm::quat& GetLocalRotation() { return mTransform.GetOrientation(); }
    const glm::vec3& GetLocalScale() { return mTransform.GetScale(); }
    const glm::mat4& GetLocalTransformMatrix() { return mTransform.GetTransformMatrix(); }
   
    
    void Render(Actor* Actor, float dt, const Shader* shader, Transform transform = Transform{});

    ///Members and Variables
    std::vector<Vertex> mVertices{};
    std::vector<Index> mIndices{};

    VAO mVAO{ 0U }; 
    VBO mVBO{ 0U };
    EBO mEBO{ 0U }; 
    Transform mTransform{};

};

