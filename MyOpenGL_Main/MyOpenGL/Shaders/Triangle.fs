#version 330 core

//Out vectors
out vec4 FragColor;  

//In vectors
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

//Texture uniforms
uniform sampler2D texture1; 
uniform bool useTex; 

//Lighting
uniform vec3 lightPos;
uniform vec3 ligthColor;
uniform vec3 objectColor;
uniform float ambientStrength;

//View position
uniform vec3 viewPos;
  
void main()
{
    vec3 viewDirection = normalize(viewPos - FragPos); 

    //Ambient Light
    vec3 ambient = ambientStrength * ligthColor; 

    //Diffuse Light
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos); 
    float diffuse = max(dot(norm,lightDirection),0.0f); 

    //Specualr Light
    float specularStrength = 0.5f;
    vec3 reflectDirection = reflect(-lightDirection,norm);
    float spec = pow(max(dot(viewDirection,reflectDirection),0.0f),32);
    vec3 specular = specularStrength * spec * ligthColor;

    vec3 phongLight = (ambient + diffuse + specular) * objectColor;

    if(useTex)
    {
        FragColor = texture(texture1, TexCoord);    
    }
    else
    {
        FragColor = vec4(phongLight,1.0); 
    }

}