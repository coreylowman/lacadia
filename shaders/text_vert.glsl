#version 400

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
layout (location = 1) in vec3 text_color;

out vec2 texture_coords;
out vec3 fragment_color;

void main() {
    gl_Position = vec4(vertex.xy, 0.0, 1.0);

    texture_coords = vertex.zw;
    fragment_color = text_color;
}
