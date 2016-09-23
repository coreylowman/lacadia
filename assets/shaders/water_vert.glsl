#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;

out vec2 fragment_texture_coords;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main() {
    gl_Position = projection_matrix * view_matrix * vec4(vertex_position, 1.0);

    fragment_texture_coords = texture_coords;
}
