#version 330 core

//Out vectors
out vec4 FragColor;  

//In vectors
in vec3 FragPos;
in vec3 Normal;
in vec3 Colour;
in vec2 TexCoord;

//Texture uniforms
uniform sampler2D texture1; 
uniform sampler2D texture2; 
uniform bool useTex; 
uniform int texType;

//Lighting
uniform bool useLight;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;

//View position
uniform vec3 viewPos;
uniform float specularStrength;
  
void main()
{
    vec3 viewDirection = normalize(viewPos - FragPos); 

    //Ambient Light
    vec3 ambient = ambientStrength * lightColor; 

    //Diffuse Light
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos); 
    float diffuse = max(dot(norm,lightDirection),0.0f); 

    //Specualr Light
    vec3 reflectDirection = reflect(-lightDirection,norm);
    float spec = pow(max(dot(viewDirection,reflectDirection),0.0f),32) * specularStrength;
    vec3 specular =  spec * lightColor;

    vec3 phongLight = (ambient + (diffuse * lightColor) + specular) * Colour;

    if(useTex)
    {
       vec4 texColor;
       switch(texType)
       {
         case 0:
             texColor = texture(texture1, TexCoord);
             break;
         case 1:
             texColor = texture(texture2, TexCoord);
             break;
         default:
            texColor = texture(texture1, TexCoord);
            break;
       }
       if (useLight)
       {
          FragColor = texColor * vec4(phongLight, 1.0);
       }
       else
       {
          FragColor = texColor; 
       }
    }
    else if(useLight)
    {
      FragColor = vec4(phongLight,1.0); 
    }
    else
    {
      FragColor = vec4(0,1.0f,1.0f,1.0f); 
    }
}