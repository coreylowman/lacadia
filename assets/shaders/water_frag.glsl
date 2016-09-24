#version 400

layout(location = 0) out vec4 color;

in vec4 clip_space;
in vec2 texture_coords;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;
uniform sampler2D distortion_texture;

uniform float move_factor;

const float wave_strength = 0.02;

void main() {
    vec2 ndc = (clip_space.xy / clip_space.w) * 0.5 + 0.5;

    vec2 reflect_tex_coords = vec2(-ndc.x, ndc.y);
    vec2 refract_tex_coords = ndc.xy;

    vec2 tex_coords1 = texture_coords;
    tex_coords1.x += move_factor;
    tex_coords1 = fract(tex_coords1);
    vec2 distortion1 = wave_strength * (texture(distortion_texture, tex_coords1).rg * 2.0 - 1.0);

    vec2 tex_coords2 = texture_coords;
    tex_coords2.x = -tex_coords2.x + move_factor;
    tex_coords2.y += move_factor;
    tex_coords2 = fract(tex_coords2);
    vec2 distortion2 = wave_strength * (texture(distortion_texture, tex_coords2).rg * 2.0 - 1.0);

    vec2 distortion = distortion1 + distortion2;

    reflect_tex_coords += distortion;
    reflect_tex_coords.x = clamp(reflect_tex_coords.x, -0.999, -0.001);
    reflect_tex_coords.y = clamp(reflect_tex_coords.y, 0.001, 0.999);

    refract_tex_coords += distortion;
    refract_tex_coords = clamp(refract_tex_coords, 0.001, 0.999);

    vec4 reflection_color = texture(reflection_texture, reflect_tex_coords);
    vec4 refraction_color = texture(refraction_texture, refract_tex_coords);

    color = mix(reflection_color, refraction_color, 0.5);
    color = mix(color, vec4(0,0.3,0.5,1), 0.2);
}
