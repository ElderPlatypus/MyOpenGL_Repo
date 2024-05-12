#version 330 core

//Out vectors
out vec4 FragColor;  

//In vectors
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;
  
void main()
{
    FragColor = texture(texture1, TexCoord);    
}