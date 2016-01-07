#ifndef INPUTS_H
#define INPUTS_H

#include <GL/glfw3.h>

typedef struct {
    int a_down, d_down, w_down, s_down;
    int space_down, space_shift_down;
    
	int left_mouse_down, right_mouse_down;

    int p_pressed, r_pressed, l_pressed;
	int numbers_pressed[10];

    double mouse_pos[2];
    double mouse_vel[2];
} Inputs;

void update_mouse(Inputs *i, GLFWwindow *w, int button, int action, int mods);
void update_keys(Inputs *i, GLFWwindow *w, int key, int scancode, int action, int mod);

#endif
