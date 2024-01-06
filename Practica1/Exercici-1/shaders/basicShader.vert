#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;
uniform vec3 newCOLOR;

out vec3 FgColor;

void main()  {
    gl_Position = TG * vec4 (vertex, 1.0);
    FgColor = newCOLOR * color;

}
