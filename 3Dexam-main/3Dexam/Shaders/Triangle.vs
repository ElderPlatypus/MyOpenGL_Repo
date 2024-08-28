#version 330 core
layout (location = 0) in vec3 aPos;      // the layout position for position variable has attribute position 0
layout (location = 1) in vec3 aNormal;   // the layout position the normals variable has attribute position 1
layout (location = 2) in vec2 aTexCoord; // the layout position the texture variable has attribute position 2
  
//Out vectors
out vec3 FragPos;  // output position to the fragment shader
out vec2 TexCoord; // output texture to the fragment shader
out vec3 Normal;   // output normals to the fragment shader

//Uniforms
uniform mat4 model;      //Positon, rotation & scaling
uniform mat4 view;       //View direction
uniform mat4 projection; //Gives perspective to the scene

void main()
{
    FragPos = vec3(model * vec4(aPos,1.0));     // Declaring FragPos as a 3D-vector and then mulitplying the content to get correct position depending on translation
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);  // Declaring the TextCoord as a 2D-vector with respect to the x & y coordinate
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);  //Declaring the Normal by normalizing a 3x3 matrix of the transposed-inverse "mdel" multiplied by aNormal to find the layout position

    gl_Position = projection * view * model * vec4(aPos,1.0); // Calculating the total layour for the rendering: position,texture and normals.
}       