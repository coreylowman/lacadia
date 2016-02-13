#ifndef INPUTS_H
#define INPUTS_H

#include <GL/glfw3.h>

typedef struct {
    int a_down, d_down, w_down, s_down;
    int space_down, space_shift_down;
    
	int left_mouse_down, right_mouse_down;
    int scroll_amount;

    int e_pressed, p_pressed, r_pressed, l_pressed;
    int tab_pressed;
    int esc_pressed;
	int numbers_pressed[10];

    double mouse_pos[2];
    double mouse_vel[2];
} Inputs;

void inputs_update_mouse_position(Inputs *i, GLFWwindow *w, double x, double y);
void inputs_update_mouse(Inputs *i, GLFWwindow *w, int button, int action, int mods);
void inputs_update_keys(Inputs *i, GLFWwindow *w, int key, int scancode, int action, int mod);
void inputs_update_scroll(Inputs *i, GLFWwindow *w, double xoffset, double yoffset);
void inputs_reset_frame(Inputs *i);

#endif
