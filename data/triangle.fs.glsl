#version 330 core

uniform vec4 color;

out vec4 fragColor;

void main() {
    // colorOut = vec4(1.0, 0.0, 0.0, 1.0);
    fragColor = color;
}
