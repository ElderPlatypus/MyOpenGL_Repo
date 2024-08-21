#version 330 core

//Out vectors
out vec4 FragColor;  

//In vectors
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1; 
uniform bool useTex; 

uniform vec3 viewPos;
  
void main()
{

    if(useTex)
    {
        FragColor = texture(texture1, TexCoord);    
    }
    else
    {
        FragColor = vec4(Normal,1.0);
    }
 

    vec3 viewDir = normalize(viewPos-FragPos);
}