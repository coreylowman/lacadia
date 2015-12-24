#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "util/inputs.h"
#include "util/camera.h"
#include "util/shaders.h"
#include "util/game_world.h"
#include "util/axis.h"
#include "util/log.h"
#include "players/player.h"
#include "players/mage.h"
#include "enemies/enemy.h"
#include "enemies/bug.h"

static GLFWwindow *window;
static int width = 1280,height = 960;

static double last_update_seconds;
static double last_fps_seconds;
static int draw_count;

static Inputs inputs;
static Shader shader;
static Camera camera;
static Axis axis;

static double mouse_start_pos[2];

LOG_TYPE log_type = NONE; //log everything
GameWorld *world;
Player *player;

static void mouse_callback(GLFWwindow *w,int button, int action, int mods)
{
    update_mouse(&inputs, w, button, action, mods);
	if (inputs.left_mouse_down) glfwGetCursorPos(w, &mouse_start_pos[0], &mouse_start_pos[1]);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
    update_keys(&inputs, window, key, scancode, action, mod);
    int i;
    for(i = 1;i < 5;i++){
        if(inputs.numbers_pressed[i]){
            player_use_ability(player, i - 1);
        }
    }
}

void window_size_callback (GLFWwindow* window, int _width, int _height) {
    width = _width;
    height = _height;
    glViewport(0, 0, width, height);
}

static void init_glfw(){
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(width, height, "lacadia", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //callbacks
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    
    glfwGetFramebufferSize(window, &width, &height);

    glEnable(GL_DEPTH_TEST);
}

static void init_glew(){
    // get version info
    glewInit();
    glewExperimental = 1; // Needed for core profile
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);
}

static void update_fps(double total_time){
    double fps_dt = total_time - last_fps_seconds;
    if (fps_dt > 0.25) {
        last_fps_seconds = total_time;
        char title[64];
        sprintf(title, "lacadia (fps: %.2f)", (double)draw_count / fps_dt);
        glfwSetWindowTitle(window, title);

        draw_count = 0;
    }
}

static void update(double total_time){
    double update_dt = total_time - last_update_seconds;
    if (update_dt > 0.01) {
        last_update_seconds = total_time;

        game_world_update(world, update_dt);

        if (inputs.space_down) { //move vertically
            double dir = inputs.space_shift_down ? -1.0 : 1.0;
            move_vertically(&camera, update_dt, dir);
        }

        if (inputs.left_mouse_down) {
            double mx, my;
            glfwGetCursorPos(window, &mx, &my);
            double dx = mx - mouse_start_pos[0];
            double dy = my - mouse_start_pos[1];
            rotate_view(&camera, dx / 100.0,-dy / 100.0);

            mouse_start_pos[0] = mx;
            mouse_start_pos[1] = my;
        }

        if (inputs.w_down) move_forwards(&camera, update_dt, 1.0);
        if (inputs.s_down) move_forwards(&camera, update_dt, -1.0);
        if (inputs.a_down) strafe(&camera, update_dt, -1.0);
        if (inputs.d_down) strafe(&camera, update_dt, 1.0);

        update_view_matrix(&camera);
    }
}

static void render(){
    draw_count += 1;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader.program);

	glUniformMatrix4fv(shader.projection_matrix_location, 1, GL_TRUE, &camera.projection_matrix.data[0]);
	glUniformMatrix4fv(shader.view_matrix_location, 1, GL_TRUE, &camera.view_matrix.data[0]);

    axis_render(axis, shader);
	game_world_render(world, shader);
}

int main(int argc, char *argv[]){
    double total_time = 0;
    double temp_time = 0;

    init_glfw();
    init_glew();
    init_shaders(&shader, "vertex_shader.glsl", "fragment_shader.glsl");
    camera_init(&camera, width, height);
    axis_init(&axis);

    world = game_world_new();

    //player
    player = mage_new(world);
    world->player = player;

    total_time = glfwGetTime();
    last_update_seconds = total_time;
    last_fps_seconds = total_time;
    while (!glfwWindowShouldClose(window))
    {
        total_time = glfwGetTime();

        update_fps(total_time);
        update(total_time);
        render();

        glfwSwapBuffers(window);
        temp_time = glfwGetTime();
        glfwPollEvents();
        temp_time = glfwGetTime() - temp_time;
        last_update_seconds += temp_time;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    player_free(world->player);
    world->player = NULL;
    game_world_free(world);
    
	_CrtDumpMemoryLeaks();

    return 0;
}

