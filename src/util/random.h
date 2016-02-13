#ifndef RANDOM_H
#define RANDOM_H

#include "vec3.h"

int random_in_rangei(int min, int max);
float random_in_rangef(float min, float max);
Vec3 random_unit_vec3();
Vec3 random_length_vec3(float length);

#endif
