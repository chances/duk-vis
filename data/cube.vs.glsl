#version 330 core

in vec3 position;
out vec4 vertexCoord;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    vertexCoord = gl_Position;
}
