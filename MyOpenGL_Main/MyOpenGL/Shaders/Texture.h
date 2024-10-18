#pragma once

//Includes
#include <glad/glad.h>
#include <iostream> 

#include "../Definitions.h"
#include "../Shaders/Shader.h"
class Texture
{
public:
	///Constructor and destructor
	Texture(const std::string& path,const std::shared_ptr<Shader>& shader);
	~Texture();
	Texture(const std::shared_ptr<Texture>&) = delete;

	///Load texture
	void LoadTexture();

	///Members
	TexID mId{ 0u };  
	std::string mPath;
	std::shared_ptr<Shader> mShader;

};
