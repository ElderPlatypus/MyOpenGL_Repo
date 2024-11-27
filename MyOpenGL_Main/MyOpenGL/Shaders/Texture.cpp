#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


Texture::Texture(const GLuint& ID, const GLenum& slot, const std::string& uniformName, const std::string& path, const std::shared_ptr<Shader>& shader) : mUniformName(uniformName), mId(ID),mSlot(slot), mPath(path), mShader(shader)  
{
    LoadTexture(); 
}

Texture::~Texture()
{
	glDeleteTextures(1, &mId); 
}

void Texture::LoadTexture()    
{
    //Load texture and apply ID according to GL_TEXTYPE(mSlot)
    glGenTextures(1, &mId); 
    glActiveTexture(mSlot);  
    glBindTexture(GL_TEXTURE_2D, mId);

    //Define channels and flip image
    int width, height, nrChannels;  
    stbi_set_flip_vertically_on_load(true); 

    // Texture Parametres
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Add data as a pointer
    unsigned char* data = stbi_load(mPath.c_str(), &width, &height, &nrChannels, 0); 
    if (data)
    {
        GLenum format = GL_RGB; 
        if (nrChannels == 1)  format = GL_RED; 
        else if (nrChannels == 4) format = GL_RGBA; 

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); 
        glGenerateMipmap(GL_TEXTURE_2D);  
    }
    else
    {
        std::cout << "Failed to load texture\n" << std::endl; 
    }
    stbi_image_free(data);

    //Apply texture
    mShader->use();
    mShader->setInt(mUniformName, static_cast<int>(mSlot - GL_TEXTURE0)); 
}

GLuint Texture::GetId() const
{
    return mId;
}


