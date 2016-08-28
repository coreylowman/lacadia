#include <stdlib.h>
#include "random.h"

int random_in_rangei(int min, int max){
    return (rand() % (max - min)) + min;
}

float random_in_rangef(float min, float max) {
    float val = ((float)rand()) / (float)RAND_MAX;
    return val * (max - min) + min;
}

Vec3 random_unit_vec3() {
    Vec3 output;
    output.x = 2 * (((float)rand()) / (float)RAND_MAX) - 1;
    output.y = (((float)rand()) / (float)RAND_MAX);
    output.z = 2 * (((float)rand()) / (float)RAND_MAX) - 1;
    vec3_normalize(&output);
    return output;
}

Vec3 random_length_vec3(float length){
    Vec3 output = random_unit_vec3();
    return vec3_scale(output, length);
}

Vec3 random_flat_vec3(float length) {
    Vec3 output = random_unit_vec3();
    output.y = 0;
    vec3_normalize(&output);
    return vec3_scale(output, length);
}
