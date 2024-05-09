#pragma once
//Includes std
#include <iostream>
#include <vector>

//Includes glm
#include "Vertex.h"
#include "../Definitions.h"
#include "../Shaders/Shader.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<Index>& indices);
    ~Mesh();

    //Mesh vertex data
    static Mesh* Create2DTriangle();
   

    //Configure and setup the vertex array
    void configureMesh();

    void drawMesh(const Shader* shader)const ; 

   
    ///Members and Variables
    std::vector<Vertex> mVertices{};
    std::vector<Index> mIndices{};

    VAO mVAO{ 0U };
    VBO mVBO{ 0U };
    EBO mEBO{ 0U }; 

};

