#version 400

layout(location = 0) in vec3 vertex_position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

out vec3 fragment_color;

void main() {
    mat4 VP = projection_matrix * view_matrix;
    gl_Position = VP * vec4(vertex_position, 1.0);

    fragment_color = vec3(1.0, 0.0, 0.0);
}
