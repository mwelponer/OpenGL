#shader vertex
#version 330 core

layout (location = 0) in vec4 position;

void main()
{
    gl_Position = position;//vec4(position.x, position.y, position.z, 1.0);
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
    //color = vec4(0.2f, 0.3f, 0.8f, 1.0f);
    color = u_Color;
}