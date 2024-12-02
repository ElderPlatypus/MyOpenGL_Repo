#version 330 core
layout (location = 0) in vec3 aPos;      // the layout position for position variable has attribute position 0
layout (location = 1) in vec3 aNormal;   // the layout position the normals variable has attribute position 1
layout (location = 2) in vec3 aColour;   // the layout position the colour variable has attribute position 2
layout (location = 3) in vec2 aTexCoord; // the layout position the texture variable has attribute position 3
  
//Out vectors
out vec3 FragPos;  // output position to the fragment shader
out vec3 Normal;   // output normals to the fragment shader
out vec3 Colour;   // output colours to the fragment shader
out vec2 TexCoord; // output texture to the fragment shader

//Uniforms
uniform mat4 model;      //Positon, rotation & scaling
uniform mat4 view;       //View direction
uniform mat4 projection; //Gives perspective to the scene

void main()
{
    FragPos = vec3(model * vec4(aPos,1.0));     // Initializing FragPos as a 3D-vector and then mulitplying the content to get correct position depending on translation
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);  //Initializing the Normal by normalizing a 3x3 matrix of the transposed-inverse "mdel" multiplied by aNormal to find the layout position
    Colour = aColour; // Initializing the colour
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);  // Initializing the TextCoord as a 2D-vector with respect to the x & y coordinate

    gl_Position = projection * view * model * vec4(aPos,1.0); // Initializing and calculating the total layout for the rendering: position,texture and normals.
}       