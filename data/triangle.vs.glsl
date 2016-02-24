#version 330 core

//in vec2 position;
//
//layout(location = 0) in vec3 vertexPosition_modelspace;
//
//void main() {
//    gl_Position = vec4(position, 0.0, 1.0);
//}

in vec2 position;

void main() {
    gl_Position = vec4( position, 0.0, 1.0 );
}
