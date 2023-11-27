#version 330 core

in vec3 vertex;
in vec3 color;

out vec3 ourColor;

uniform mat4 TG;
uniform vec3 colorAsp;
uniform float escala;

void main()  {
    gl_Position = TG * vec4 ( vertex * escala, 1.0);
    ourColor = color * colorAsp;

}