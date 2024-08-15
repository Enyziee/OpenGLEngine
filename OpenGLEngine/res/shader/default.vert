#version 460 core

layout (location = 0) in vec3 a_Position;
//layout (location = 1) in vec2 a_TexCoord;

out vec4 ourColor;
//out vec2 TexCoord;

//uniform mat4 u_View;
//uniform mat4 u_Projection;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main() {
    //gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
    //gl_Position = vec4(a_Position, 1.0);
    ourColor = vec4(1.0, 1.0, 1.0, 1.0);
    //TexCoord = a_TexCoord;
}