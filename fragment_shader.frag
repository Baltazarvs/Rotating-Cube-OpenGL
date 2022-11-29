#version 330 core

in vec3 out_color;
out vec4 oColor;

void main()
{
    oColor = vec4(out_color.x, out_color.y, out_color.z, 1.0);
}