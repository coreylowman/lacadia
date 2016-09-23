#version 400

layout(location = 0) out vec4 color;

in vec4 clip_space;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;

void main() {
    vec2 ndc = (clip_space.xy / clip_space.w) * 0.5 + 0.5;

    vec2 reflect_tex_coords = vec2(-ndc.x, ndc.y);
    vec2 refract_tex_coords = ndc.xy;

    vec4 reflection_color = texture(reflection_texture, reflect_tex_coords);
    vec4 refraction_color = texture(refraction_texture, refract_tex_coords);

    color = mix(reflection_color, refraction_color, 0.5);
}
