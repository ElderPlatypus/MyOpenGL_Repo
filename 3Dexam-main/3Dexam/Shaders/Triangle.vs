#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord; // the texture variable has  attribute position 2
  
//Out vectors
out vec3 FragPos; // output a color to the fragment shader
out vec2 TexCoord;
out vec3 Normal;

//Uniforms
uniform mat4 model; //Positon, rotation & scaling
uniform mat4 view; //View direction
uniform mat4 projection; //Gives perspective to the scene

void main()
{
    gl_Position = projection * view * model * vec4(aPos,1.0);
    FragPos = vec3(model*vec4(aPos,1.0)); // set FragPos to the input color we got from the vertex data
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);  
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
}       