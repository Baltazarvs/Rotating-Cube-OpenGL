#version 330 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColor;

out vec3 out_color;
uniform mat4 new_pos;

void main()
{
    out_color = iColor;
    gl_Position = new_pos * vec4(iPosition, 1.0);
}