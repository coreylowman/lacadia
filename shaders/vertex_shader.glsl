#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in mat4 model_matrix;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

out vec3 fragment_color;

void main() {
    mat4 MVP = projection_matrix * view_matrix * model_matrix;
    gl_Position = MVP * vec4(vertex_position, 1.0);

    fragment_color = vertex_color;
}
