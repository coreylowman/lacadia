#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "util/inputs.h"
#include "util/camera.h"
#include "util/shader.h"
#include "util/random.h"
#include "game/game_world.h"
#include "players/player.h"
#include "players/mage.h"
#include "enemies/enemy.h"
#include "enemies/bug.h"
#include "game/level.h"

static GLFWwindow *window;
int width = 1280,height = 960;

static double last_update_seconds;
static double last_fps_seconds;
static int draw_count;

GameWorld *world;

static void mouse_callback(GLFWwindow *w,int button, int action, int mods)
{
    inputs_update_mouse(&world->inputs, w, button, action, mods);
}

static void mouse_position_callback(GLFWwindow *w, double x, double y){
	inputs_update_mouse_position(&world->inputs, w, x, y);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mod){
    inputs_update_keys(&world->inputs, window, key, scancode, action, mod);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    inputs_update_scroll(&world->inputs, window, xoffset, yoffset);
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
    
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //callbacks
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, mouse_position_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
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
    }
}

static void render(){
    draw_count += 1;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    game_world_render(world);
}

int main(int argc, char *argv[]){
    double total_time = 0;
    double temp_time = 0;

    srand(time(NULL));

    init_glfw();
    init_glew();

    //sky blue
    glClearColor(0.52734375, 0.8046875, 0.91796875, 1.0);

    world = game_world_new();
    game_world_add_object(world, mage_new(world));

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

    game_world_free(world);

    // for debugging only
    _CrtDumpMemoryLeaks();

    return 0;
}

