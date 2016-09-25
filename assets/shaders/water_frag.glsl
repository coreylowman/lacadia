#version 400

layout(location = 0) out vec4 color;

in vec4 clip_space;
in vec2 texture_coords;
in vec3 to_camera;
in vec3 from_light;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;
uniform sampler2D distortion_texture;
uniform sampler2D normal_texture;
uniform sampler2D depth_texture;

uniform float move_factor;

const float wave_strength = 0.04;
const float shine_damp = 20.0;
const float reflectivity = 0.5;
const vec3 light_color = vec3(1, 1, 1);

void main() {
    vec2 ndc = (clip_space.xy / clip_space.w) * 0.5 + 0.5;

    vec2 reflect_tex_coords = vec2(-ndc.x, ndc.y);
    vec2 refract_tex_coords = ndc.xy;

    // depth calculation
    float near = 0.1;
    float far = 1000.0;
    float depth = texture(depth_texture, refract_tex_coords).r;
    float floor_dist = 2.0 * near * far / (far + near - (2 * depth - 1.0) * (far - near));

    depth = gl_FragCoord.z;
    float water_dist = 2.0 * near * far / (far + near - (2 * depth - 1.0) * (far - near));

    float water_depth = floor_dist - water_dist;
    float water_depth_factor = clamp(water_depth / 5.0, 0, 1);

    // distortion calculation
    vec2 normalized_tex_coords = fract(texture_coords);
    vec2 move_x = fract(vec2(normalized_tex_coords.x + move_factor, normalized_tex_coords.y));

    vec2 tex_coords = texture(distortion_texture, move_x).xy * 0.1;
    tex_coords = fract(normalized_tex_coords + vec2(tex_coords.x, tex_coords.y + move_factor));
    vec2 distortion = wave_strength * (texture(distortion_texture, tex_coords).xy * 2.0 - 1.0) * water_depth_factor;

    // tex_coords distortion
    reflect_tex_coords += distortion;
    reflect_tex_coords.x = clamp(reflect_tex_coords.x, -0.999, -0.001);
    reflect_tex_coords.y = clamp(reflect_tex_coords.y, 0.001, 0.999);

    refract_tex_coords += distortion;
    refract_tex_coords = clamp(refract_tex_coords, 0.001, 0.999);

    // sampling
    vec4 reflection_color = texture(reflection_texture, reflect_tex_coords);
    vec4 refraction_color = texture(refraction_texture, refract_tex_coords);

    // normal calculation
    vec4 normal_color = texture(normal_texture, tex_coords);
    vec3 normal = vec3(normal_color.r * 2.0 - 1.0, normal_color.b * 5.0, normal_color.g * 2.0 - 1.0);
    normal = normalize(normal);

    // frennel calculation
    vec3 view = normalize(to_camera);
    float refractive_factor = dot(view, normal);
    refractive_factor = pow(refractive_factor, 0.5);

    // specular highlight calculation
    vec3 reflected_light = reflect(normalize(from_light), normal);
    float specular = max(dot(reflected_light, view), 0.0);
    specular = pow(specular, shine_damp);
    vec3 specular_highlights = light_color * specular * reflectivity * water_depth_factor;

    // output color calculation
    color = mix(reflection_color, refraction_color, refractive_factor);
    color = mix(color, vec4(0,0.3,0.5,1), 0.2);
    color += vec4(specular_highlights, 0.0);
    color.a = water_depth_factor;
}
