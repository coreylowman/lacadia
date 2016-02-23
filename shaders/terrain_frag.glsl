#version 400

in vec3 fragment_color;
in vec3 fragment_position;
in vec3 fragment_normal;
in vec3 light_position_camera;

//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/
out vec4 color;

void main() {
    vec3 light_color = vec3(1.0, 1.0, 1.0);
    float light_power = 0.75;

    vec3 n = normalize(fragment_normal);
    vec3 l = normalize(light_position_camera - fragment_position);
    float cosTheta = clamp(dot(n, l), 0.0, 1.0);
    float dist = length(l);
    dist *= dist;
    vec3 e = normalize(fragment_position);
    vec3 r = reflect(l, n);
    float cosAlpha = clamp(dot(e, r), 0.0, 1.0);

    //ambient color
    vec3 fcolor = vec3(0.1, 0.1, 0.1) * fragment_color;

    //diffuse color
    fcolor += fragment_color * light_color * light_power * cosTheta / dist;

    //specular color
    fcolor += 0.25 * light_color * light_power * pow(cosAlpha, 5) / dist;

    color = vec4(fcolor, 1.0);
}
