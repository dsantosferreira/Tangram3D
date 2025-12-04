#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

uniform vec4 Color;

out vec4 FragmentColor;

vec3 normalColor(void) {
    return (exNormal * vec3(1.0)) * 0.1;
}

void main(void)
{
    FragmentColor = Color + vec4(normalColor(), 1.0);
}
