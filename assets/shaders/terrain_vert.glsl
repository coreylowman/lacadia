#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 vertex_color;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 light_position;

out vec3 fragment_position;
out vec3 fragment_normal;
out vec3 fragment_color;
out vec3 light_position_camera;

void main() {
    mat4 VP = projection_matrix * view_matrix;
    gl_Position = VP * vec4(vertex_position, 1.0);

    fragment_position = (view_matrix * vec4(vertex_position, 1.0)).xyz;
    fragment_normal = normalize((view_matrix * vec4(vertex_normal, 0.0)).xyz);
    fragment_color = vertex_color;
    light_position_camera = (view_matrix * vec4(light_position, 1.0)).xyz;
}
