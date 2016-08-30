#ifndef INPUTS_H
#define INPUTS_H

#include <GL/glfw3.h>

// TODO rename this Input
// TODO get rid of the _down fields (only need for tracking mouse movement while its down...)
// TODO add all keys
typedef struct Inputs {
    int a_down, d_down, w_down, s_down;
    int up_down, down_down, left_down, right_down;
    int space_down, space_shift_down;

    double mouse_down_start_pos[2];
	int left_mouse_down, right_mouse_down;
    int scroll_amount;

    int q_pressed, e_pressed, p_pressed, r_pressed, l_pressed;
    int m_pressed;
    int tab_pressed;
    int esc_pressed;
	int numbers_pressed[10];

    double mouse_pos[2];
    double mouse_vel[2];
} Inputs;

Inputs inputs_init();
void inputs_update_mouse_position(Inputs *i, GLFWwindow *w, double x, double y);
void inputs_update_mouse(Inputs *i, GLFWwindow *w, int button, int action, int mods);
void inputs_update_keys(Inputs *i, GLFWwindow *w, int key, int scancode, int action, int mod);
void inputs_update_scroll(Inputs *i, GLFWwindow *w, double xoffset, double yoffset);
void inputs_reset_frame(Inputs *i);

#endif
