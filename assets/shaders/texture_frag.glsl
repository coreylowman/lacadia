#version 400

in vec2 texture_coords;

out vec4 color;

uniform sampler2D tex;

void main() {
    color = texture(tex, texture_coords);
}
