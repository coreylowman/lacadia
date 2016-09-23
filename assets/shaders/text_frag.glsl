#version 400

uniform sampler2D text;

in vec2 texture_coords;
in vec3 fragment_color;

out vec4 color;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(text, texture_coords).r);
    color = vec4(fragment_color, 1.0) * sampled;
}
