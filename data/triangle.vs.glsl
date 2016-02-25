#version 330 core

in vec3 position;
out vec4 vertexCoord;

uniform mat4 model;

void main() {
    gl_Position = vec4(position, 1.0) * model;
    vertexCoord = gl_Position;
}
