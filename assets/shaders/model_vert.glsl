#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in mat4 model_matrix;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 light_position;
uniform vec4 clip_plane;

out vec4 fragment_color;
out vec3 fragment_position;
out vec3 fragment_normal;
out vec3 light_position_camera;

void main() {
    vec4 world_position = model_matrix * vec4(vertex_position, 1.0);
    gl_ClipDistance[0] = dot(world_position, clip_plane);

    vec4 camera_rel_position = view_matrix * world_position;

    gl_Position = projection_matrix * camera_rel_position;

    fragment_color = vec4(vertex_color, 1.0);
    fragment_position = camera_rel_position.xyz;
    light_position_camera = (view_matrix * vec4(light_position, 1.0)).xyz;
    fragment_normal = normalize((view_matrix * model_matrix * vec4(vertex_normal, 0.0)).xyz);
}
