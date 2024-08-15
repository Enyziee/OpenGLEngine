#version 460 core

out vec4 FragColor;
  
in vec4 ourColor;
//in vec2 TexCoord;

//uniform sampler2D u_Texture1;
//uniform sampler2D u_Texture2;

void main() {
     // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
     FragColor = ourColor;
}