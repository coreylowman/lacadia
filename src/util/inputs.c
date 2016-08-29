#include <stdio.h>
#include "inputs.h"

Inputs inputs_init() {
    Inputs self;

    self.a_down = 0;
    self.d_down = 0;
    self.w_down = 0;
    self.s_down = 0;

    self.up_down = 0;
    self.down_down = 0;
    self.left_down = 0;
    self.right_down = 0;

    self.space_down = 0;
    self.space_shift_down = 0;

    self.mouse_down_start_pos[0] = 0;
    self.mouse_down_start_pos[1] = 0;
    
    self.left_mouse_down = 0;
    self.right_mouse_down = 0;
    
    self.scroll_amount = 0;

    
    self.q_pressed = 0;
    self.e_pressed = 0;
    self.p_pressed = 0;
    self.r_pressed = 0;
    self.l_pressed = 0;
    
    self.tab_pressed = 0;
    
    self.esc_pressed = 0;

    int i;
    for(i = 0;i < 10;i++){
        self.numbers_pressed[i] = 0;
    }

    self.mouse_pos[0] = 0;
    self.mouse_pos[1] = 0;

    self.mouse_vel[0] = 0;
    self.mouse_vel[1] = 0;

	return self;
}

void inputs_update_mouse(Inputs *i, GLFWwindow *w,int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        i->left_mouse_down = 1;
        i->mouse_down_start_pos[0] = i->mouse_pos[0];
        i->mouse_down_start_pos[1] = i->mouse_pos[1];
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        i->left_mouse_down = 0;
    }

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
		i->right_mouse_down = 1;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		i->right_mouse_down = 0;
	}
}

void inputs_update_mouse_position(Inputs *i, GLFWwindow *w, double x, double y){
	i->mouse_vel[0] = x - i->mouse_pos[0];
	i->mouse_vel[1] = y - i->mouse_pos[1];
	i->mouse_pos[0] = x;
	i->mouse_pos[1] = y;
}

void inputs_update_keys(Inputs *i, GLFWwindow *window, int key, int scancode, int action, int mod){
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) i->numbers_pressed[1] = 1;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) i->numbers_pressed[2] = 1;
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) i->numbers_pressed[3] = 1;
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) i->numbers_pressed[4] = 1;

    if(key == GLFW_KEY_Q && action == GLFW_PRESS) i->q_pressed = 1;
    if(key == GLFW_KEY_P && action == GLFW_PRESS) i->p_pressed = 1;
    if(key == GLFW_KEY_R && action == GLFW_PRESS) i->r_pressed = 1;
    if(key == GLFW_KEY_L && action == GLFW_PRESS) i->l_pressed = 1;
    if(key == GLFW_KEY_E && action == GLFW_PRESS) i->e_pressed = 1;
    if(key == GLFW_KEY_TAB && action == GLFW_PRESS) i->tab_pressed = 1;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) i->esc_pressed = 1;

    if (key == GLFW_KEY_A && action == GLFW_PRESS) i->a_down = 1;
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE) i->a_down = 0;

    if (key == GLFW_KEY_D && action == GLFW_PRESS) i->d_down = 1;
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE) i->d_down = 0;
    
    if (key == GLFW_KEY_W && action == GLFW_PRESS) i->w_down = 1;
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE) i->w_down = 0;

    if (key == GLFW_KEY_S && action == GLFW_PRESS) i->s_down = 1;
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE) i->s_down = 0;

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) i->up_down = 1;
    else if (key == GLFW_KEY_UP && action == GLFW_RELEASE) i->up_down = 0;

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) i->down_down = 1;
    else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) i->down_down = 0;
    
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) i->left_down = 1;
    else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) i->left_down = 0;

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) i->right_down = 1;
    else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) i->right_down = 0;

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        if(mod == GLFW_MOD_SHIFT) i->space_shift_down = 1;
        i->space_down = 1;
    } else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        i->space_down = 0;
        i->space_shift_down = 0;
    }
}

void inputs_update_scroll(Inputs *i, GLFWwindow *w, double xoffset, double yoffset){
    i->scroll_amount = yoffset;
}

void inputs_reset_frame(Inputs *i){
    i->p_pressed = 0;
    i->r_pressed = 0;
    i->l_pressed = 0;
    i->q_pressed = 0;
    i->e_pressed = 0;
    i->tab_pressed = 0;

    int j;
    for(j = 0;j < 10;j++){
        i->numbers_pressed[j] = 0;
    }
    
    i->mouse_vel[0] = 0;
    i->mouse_vel[1] = 0;
    i->scroll_amount = 0;
}
