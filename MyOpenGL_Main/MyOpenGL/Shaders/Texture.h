#pragma once

//Includes c++
#include <iostream> 

//External Includes
#include <glad/glad.h>

//Full Inclusion
#include "../Definitions.h"
#include "../Shaders/Shader.h"
class Texture
{
public:
	///Constructor and destructor
	Texture(const GLuint& ID,const GLenum& slot, const std::string& uniformName, const std::string& path,const std::shared_ptr<Shader>& shader);
	~Texture();
	Texture(const std::shared_ptr<Texture>&) = delete;

	///Load texture
	void LoadTexture(); 
	GLuint GetId() const;

	///Members
private:
	GLuint mId;
	GLenum mSlot;
	std::string mPath;
	std::string mUniformName;
	std::shared_ptr<Shader> mShader;

};
