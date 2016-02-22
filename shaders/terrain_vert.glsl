#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

out vec3 fragment_color;
out vec3 fragment_position;
out vec3 fragment_normal;
out vec3 light_position;

void main() {
    mat4 VP = projection_matrix * view_matrix;
    gl_Position = VP * vec4(vertex_position, 1.0);

    fragment_color = vec3(1.0, 1.0, 1.0);
    fragment_position = (view_matrix * vec4(vertex_position, 1.0)).xyz;
    light_position = (view_matrix * vec4(0.0, 10.0, 0.0, 1.0)).xyz;
    fragment_normal = normalize((view_matrix * vec4(vertex_normal, 0.0)).xyz);
}
