#version 330 core

out vec4 FragColor;

in vec3 FgColor;

void main() {
    FragColor = vec4(FgColor, 1);
}
