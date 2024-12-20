#include "Mesh.h"

///Constructor and Destructor
Mesh::Mesh(const std::string& type, const std::vector<Vertex>& vertices,
           const std::vector<Index>& indices, const bool& useTex,const GLDrawType& GLdrawType)
           : mVertices(vertices), mIndices(indices), mType(type), drawType(GLdrawType)
{
    configureMesh();

    UpdateBounds();
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

void Mesh::updateVertexAttribs()
{
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(Index), mIndices.data(), GL_STATIC_DRAW);
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
        case GLDrawType::Triangle:
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);  break;
        case GLDrawType::Line_Strip:
            glDrawElements(GL_LINE_STRIP, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;
        case GLDrawType::Points:
            glDrawElements(GL_POINTS, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;
        case GLDrawType::Triangle_Fan:
            glDrawElements(GL_TRIANGLE_FAN, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;
        default:
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0); break;
        }
    }
}

void Mesh::UpdateBounds()
{

    if (mVertices.empty())
    {
        std::cerr << "[WARNING]:No vertices detected \n";
        return;
    }
    minExtent = mVertices[0].mPos;
    maxExtent = mVertices[0].mPos;
    for (const auto& vert : mVertices) 
    {
        minExtent = glm::min(minExtent, vert.mPos);
        maxExtent = glm::max(maxExtent, vert.mPos);
    }

    mExtent = (maxExtent - minExtent) * 0.5f;
    mCenter = (minExtent + maxExtent) * 0.5f;
}

void Mesh::TexConfig(const bool& useTexture, const TextureType& texType)
{
    mUseTex = useTexture;
    mTexType = texType;
    mShader->use();
    mShader->setBool("useTex", useTexture);
    mShader->setInt("texType", (int)mTexType);
}

void Mesh::SetShaderDefaults(bool useLight) const 
{
    mShader->setBool("useLight", useLight);
    mShader->setVec3("lightColor", glm::vec3(0.0f, 0.5f, 1.0f));
    mShader->setVec3("lightPos", glm::vec3(0.f, 100.f, 0.f));
    mShader->setVec3("objectColor", glm::vec3(1.f, 0.f, 0.0f));
    mShader->setFloat("ambientStrength", 5.f);
}

void Mesh::CalculateSlopeColour()
{
    //Initialising local variables
    float min = 0;
    float max = 0;
    float average = 0;
    for (size_t i = 0; i < mVertices.size(); i++)
    {
        //Initialising the variable for the ground normal
        glm::vec3 groundNormal(0.f, 1.f, 0.f); 

        //Calcuating dot product and 
        float dotProduct = glm::dot(mVertices[i].mNormals, groundNormal); 
        float magnitude = glm::length(mVertices[i].mNormals) * glm::length(groundNormal); 

        //Convertig angle to radian
        const float radianConvert = 180.f / 3.14f;

        //Using arccos to find angle on slope and converting to radian
        float angle = asin(dotProduct / magnitude) * radianConvert; 

        //Calulating min and max and average angle
        min = glm::min(min, angle);
        max = glm::max(max, angle); 
        average = (min + max) / 2.0f;
        //std::cout << "[LOG]:Average is:" << average << "\n";
        //std::cout << "[LOG]:Angle is:" << angle << "\n";

        //Contitions for applying colour
        if (angle < average)  
        {
            mVertices[i].mColour = glm::vec3(0.5f, 0.35f, 0.05f); // Brown
        }
        else if (angle == average) 
        {
            mVertices[i].mColour = glm::vec3(0.7f, 0.7f, 0.7f); // Grey 
        }
        else
        {
            mVertices[i].mColour = glm::vec3(0.4f, 0.4f, 0.4f); // Dim Grey
        }
    }
    updateVertexAttribs();  
}

void Mesh::LightConfig(const bool& useLight, const LightType& lightType) 
{
    //Updating members
    mUseLight = useLight;
    mlightType = lightType;

    //Safe check if light is being used for mesh
    if (mUseLight) 
    {
        //Updating shader
        mShader->use();
        mShader->setBool("useLight", mUseLight); 
        mShader->setVec3("lightColor", mLightColor);
        mShader->setVec3("lightPos", mLightPos);
        mShader->setFloat("ambientStrength", mAmbientStrength);
        mShader->setFloat("specularStrength", mSpecularStrength);

        //Selecting light type
        switch (lightType)
        {
        case LightType::Default: 
            mShader->setVec3("objectColor", mObjectColor);
            break;
        case LightType::Slope:
        {
            CalculateSlopeColour();
            mShader->setVec3("objectColor", slopeColour);
            break;
        }
        default:
            SetShaderDefaults(mUseLight);
            break;
        }
    }
    else
    {
        SetShaderDefaults(mUseLight);
    }
}


