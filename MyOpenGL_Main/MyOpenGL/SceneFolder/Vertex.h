#pragma once

//External
#include <glad/glad.h>
#include <glm/glm.hpp>

class Vertex
{
public:
    glm::vec3 mPos;
    glm::vec3 mNormals;
    glm::vec2 mTextCoords;

    Vertex(float x, float y, float z)
    {
        mPos = glm::vec3(x, y, z);
        mNormals = glm::vec3(1.f);
        mTextCoords = glm::vec2(1.f);
    }

    Vertex(float x, float y, float z, float nX, float nY, float nZ, float u, float v) 
    {
        mPos = glm::vec3(x, y, z);
        mNormals = glm::vec3(nX,nY,nZ);
        mTextCoords = glm::vec2(u,v); 
    }


    Vertex(const glm::vec3& position, const glm::vec3& normals, const glm::vec2 textCoords)
    {
        mPos = position;
        mNormals = normals;
        mTextCoords = textCoords;
    }
     

    static void configureVertexAttribs()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mPos));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormals));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTextCoords));
        glEnableVertexAttribArray(2);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        // glBindVertexArray(0);
    }
};