#version 330 core

//Out vectors
out vec4 FragColor;  

//In vectors
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 viewPos;
  
void main()
{
    FragColor = texture(texture1, TexCoord);    
    vec3 viewDir = normalize(viewPos-FragPos);
}