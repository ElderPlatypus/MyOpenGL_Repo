#version 330 core

//Out vectors
out vec4 FragColor;  

//In vectors
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

//Texture uniforms
uniform sampler2D texture1; 
uniform sampler2D texture2; 
uniform bool useTex; 
uniform int texType;

//Lighting
uniform bool useLight;
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
       switch(texType)
       {
         case 0:
           if(useLight) 
             {
                FragColor = texture(texture1, TexCoord) * vec4(phongLight,1.0);
             }
           else 
             { 
                FragColor = texture(texture1, TexCoord);
             }
             break;
         case 1:
             if(useLight) 
             {
                FragColor = texture(texture2, TexCoord) * vec4(phongLight,1.0);
             }
           else 
             { 
                FragColor = texture(texture2, TexCoord);
             }
             break;
         default:
           FragColor = texture(texture1, TexCoord);
           break;
       }
    }
    else if(useLight)
    {
      FragColor = vec4(phongLight,1.0); 
    }
    else
    {
      FragColor = vec4(0,0,1,1.0f); 
    }
}