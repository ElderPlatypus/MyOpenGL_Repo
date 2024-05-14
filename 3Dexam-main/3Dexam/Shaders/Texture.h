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
	Texture(const std::string& path, Shader* shader);
	~Texture();
	Texture(const Texture&) = delete;

	///Load texture
	void LoadTexture();

	///Members
	TexID mId{ 0u };  
	std::string mPath;
	Shader* mShader{ nullptr };

};
