#version 400

in vec2 fragment_texture_coords;

layout(location = 0) out vec4 color;

uniform sampler2D text;

void main() {
    color = vec4(0, 0, 1, 1);
}
