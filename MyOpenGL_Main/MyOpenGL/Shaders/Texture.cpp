#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


Texture::Texture(const std::string& path, const std::shared_ptr<Shader>& shader) : mId(0), mPath(path), mShader(shader)
{
    LoadTexture();
}

Texture::~Texture()
{
	glDeleteTextures(1, &mId); 
}

void Texture::LoadTexture()
{
    ///Load correct texture to be used
    glGenTextures(1, &mId); 
    glBindTexture(GL_TEXTURE_2D, mId);

    int width, height, nrChannels; 
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* data = stbi_load(mPath.c_str(), &width, &height, &nrChannels, 0); 
    if (data)
    {
        GLenum format = GL_RGB; 
        if (nrChannels == 1) 
            format = GL_RED; 
        else if (nrChannels == 4) 
            format = GL_RGBA; 

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); 
        glGenerateMipmap(GL_TEXTURE_2D);  
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl; 
    }
    stbi_image_free(data);

    //Call shader and use it to set texture, then loading it to memory
    mShader->use();
    mShader->setInt("texture1", 0);
}


