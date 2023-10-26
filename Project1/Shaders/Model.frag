#version 330 core
out vec4 FragColor;
in vec3 ourColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

  uniform sampler2D diffuse;

void main()
{
   


    FragColor = vec4(ourColor , 1.0);
}
