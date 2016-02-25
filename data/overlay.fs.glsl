#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform mat4 model;
uniform mat4 projection;

uniform sampler2D image;
uniform vec4 penColor;

void main() {
    // vec3 fragColor;
    // fragColor.x = TexCoords.x;
    // fragColor.y = TexCoords.y;
    // fragColor.x = TexCoords.x / TexCoords.y;
    // color = vec4(fragColor, 1.0);

    color = penColor * texture(image, TexCoords);
}
