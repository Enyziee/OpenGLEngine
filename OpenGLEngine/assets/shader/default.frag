#version 460 core

out vec4 FragColor;
  
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D u_Texture1;


uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;

void main() {

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;

    vec3 result = ambient * vec3(1, 0.0, 0.0);

    FragColor = vec4(result, 1.0);

    //FragColor = texture(u_Texture1, TexCoord);
}