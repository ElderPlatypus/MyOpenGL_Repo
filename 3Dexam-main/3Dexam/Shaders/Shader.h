#ifndef SHADER_H
#define SHADER_H
#include "glad/glad.h" // include glad to get the required OpenGL headers
#include "GLFW/glfw3.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//Needed oncludes to run glUniformMatrix
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// the program ID
	unsigned int ID;
	unsigned int vertex, fragment;

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string& uniform_name, bool value) const;
	void setInt(const std::string& uniform_name, int value) const;
	void setFloat(const std::string& uniform_name, float value) const;

    // activate the shader
    // ------------------------------------------------------------------------
    void use() const
    {
        glUseProgram(ID);
    }
   
    // ------------------------------------------------------------------------
    void setVec2(const std::string& uniform_name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ID, uniform_name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& uniform_name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, uniform_name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& uniform_name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, uniform_name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& uniform_name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, uniform_name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& uniform_name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(ID, uniform_name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& uniform_name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ID, uniform_name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& uniform_name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, uniform_name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& uniform_name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, uniform_name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& uniform_name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, uniform_name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }


private:
    void checkForCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif
