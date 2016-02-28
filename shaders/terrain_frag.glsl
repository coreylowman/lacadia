#version 400

uniform sampler2D tex;

in vec2 texture_coords;
in vec3 fragment_position;
in vec3 fragment_normal;
in vec3 light_position_camera;

//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
out vec4 color;

void main() {
    vec4 light_color = vec4(1.0, 1.0, 1.0, 1.0);
    float light_power = 0.75;

    vec4 fragment_color = texture(tex, texture_coords);

    vec3 n = normalize(fragment_normal);
    vec3 l = normalize(light_position_camera - fragment_position);
    float cosTheta = clamp(dot(n, l), 0.0, 1.0);
    float dist = length(l);
    dist *= dist;
    vec3 e = normalize(fragment_position);
    vec3 r = reflect(l, n);
    float cosAlpha = clamp(dot(e, r), 0.0, 1.0);

    //ambient color
    color = vec4(0.5, 0.5, 0.5, 1.0) * fragment_color;

    //diffuse color
    color += fragment_color * light_color * light_power * cosTheta / dist;

    //specular color
    // color += 0.25 * light_color * light_power * pow(cosAlpha, 5) / dist;

    color.a = 1.0;
}
