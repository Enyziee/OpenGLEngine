#version 460 core

out vec4 FragColor;

//in vec2 v_TexCoord;
in vec3 v_Normal;

uniform vec3 u_ObjectColor;

void main() {
     FragColor = vec4(u_ObjectColor, 1.0f);
}