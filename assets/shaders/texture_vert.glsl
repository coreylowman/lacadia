#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_tex_coords;

out vec2 texture_coords;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main() {
    //gl_Position = projection_matrix * view_matrix * vec4(vertex_position, 1.0);
    gl_Position = vec4(vertex_position, 1.0);

    texture_coords = vertex_tex_coords;
}
