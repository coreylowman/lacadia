#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 tex_coords;

out vec4 clip_space;
out vec2 texture_coords;
out vec3 to_camera;
out vec3 from_light;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform vec3 camera_position;
uniform vec3 light_position;

const float tiling = 4.0;

void main() {
    clip_space = projection_matrix * view_matrix * vec4(vertex_position, 1.0);
    gl_Position = clip_space;
    texture_coords = tex_coords * tiling;
    to_camera = camera_position - vertex_position;
    from_light = vertex_position - light_position;
}
