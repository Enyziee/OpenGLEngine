#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
//layout (location = 2) in vec2 a_TexCoord;

//out vec2 v_TexCoord;
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main() {
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
    //v_TexCoord = a_TexCoord;
    v_Normal = a_Normal;
}