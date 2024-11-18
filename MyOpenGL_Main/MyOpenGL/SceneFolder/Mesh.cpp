#include "Mesh.h"

///Constructor and Destructor
Mesh::Mesh(const std::string& type, const std::vector<Vertex>& vertices,
           const std::vector<Index>& indices, const bool& useTex,
           const GLDrawType& GLdrawType) : mVertices(vertices), mIndices(indices), mType(type), drawType(GLdrawType)
{
    configureMesh();

    for (auto& it : vertices)
    {
        minExtent = glm::min(minExtent, it.mPos);
        maxExtent = glm::max(maxExtent, it.mPos);
    }

    mExtent = maxExtent - minExtent;
}


Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
}

///Configuring the mesh
void Mesh::configureMesh()
{
    //// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(Index), mIndices.data(), GL_STATIC_DRAW);
    Vertex::configureVertexAttribs();

    glBindVertexArray(0);
}

///Drawing the mesh
void Mesh::drawActor(const std::shared_ptr<Shader>& shader) const 
{
    if (!shader) { assert(shader && "No shader found\n"); return; }
    else
    {
        glBindVertexArray(mVAO);
        switch (drawType)
        {
        case Triangle: 
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;
        case Line_Strip:
            glDrawElements(GL_LINE_STRIP, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;
        case Points:
            glDrawElements(GL_POINTS, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;
        case Triangle_Fan: 
            glDrawElements(GL_TRIANGLE_FAN, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;
        default:
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;

        }
    }
}


