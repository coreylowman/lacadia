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
#include "util/shaders.h"
#include "util/axis.h"
#include "util/random.h"
#include "game/game_world.h"
#include "players/player.h"
#include "players/mage.h"
#include "enemies/enemy.h"
#include "enemies/bug.h"
#include "game/level.h"

static GLFWwindow *window;
static int width = 1280,height = 960;

static double last_update_seconds;
static double last_fps_seconds;
static int draw_count;

static Inputs inputs;
static Shader shader;
static Shader line_shader;
static Shader ui_shader;
static Camera camera;
static Axis axis;

GameWorld *world;
Player *player;

extern Mat4 MAT4_IDENT;

static void mouse_callback(GLFWwindow *w,int button, int action, int mods)
{
    inputs_update_mouse(&inputs, w, button, action, mods);
}

static void mouse_position_callback(GLFWwindow *w, double x, double y){
	inputs_update_mouse_position(&inputs, w, x, y);
}

static void follow(){
	camera_set_follow(&camera, &player->moveable, 0.75 * player->collidable.bounding_box.radius.y);
}

static void unfollow(){
	camera_set_follow(&camera, NULL, 0);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mod){
    inputs_update_keys(&inputs, window, key, scancode, action, mod);

    if (inputs.l_pressed){
		if (!camera_is_following(camera))
			follow();
		else
			unfollow();
    }

    if(inputs.e_pressed){
        game_world_add_enemy(world, bug_new(world, player->moveable.position));
    }

    if(inputs.tab_pressed){
        if(camera.follow_dist == 1.0){
			camera.follow_dist = 30.0;
        }else{
            camera.follow_dist = 1.0;
        }
    }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    inputs_update_scroll(&inputs, window, xoffset, yoffset);
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

        if(!camera_is_following(camera))
            camera_handle_inputs(&camera, update_dt, inputs);
		else{
			player_handle_inputs(player, update_dt, inputs);
			camera_follow(&camera, update_dt, inputs);
		}
        
		game_world_update(world, update_dt);

        inputs_reset_frame(&inputs);
    }
}

static void render(){
    mat4_mul(&world->world_to_screen, camera.projection_matrix, camera.view_matrix);
    
    draw_count += 1;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader.program);

    glUniformMatrix4fv(shader.projection_matrix_location, 1, GL_TRUE, &camera.projection_matrix.data[0]);
    glUniformMatrix4fv(shader.view_matrix_location, 1, GL_TRUE, &camera.view_matrix.data[0]);

    axis_render(axis, shader);
    game_world_render(world, shader);

    glUseProgram(ui_shader.program);
    game_world_render_ui(world, ui_shader);

    glUseProgram(line_shader.program);
	glUniformMatrix4fv(shader.projection_matrix_location, 1, GL_TRUE, &camera.projection_matrix.data[0]);
	glUniformMatrix4fv(shader.view_matrix_location, 1, GL_TRUE, &camera.view_matrix.data[0]);
	game_world_debug_render(world, line_shader);
}

int main(int argc, char *argv[]){
    double total_time = 0;
    double temp_time = 0;

    srand(time(NULL));

    init_glfw();
    init_glew();
    init_shaders(&shader, "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    init_shaders(&line_shader, "shaders/line_vertex_shader.glsl", "shaders/line_fragment_shader.glsl");
    init_shaders(&ui_shader, "shaders/ui_vertex_shader.glsl", "shaders/ui_fragment_shader.glsl");
    camera_init(&camera, width, height);
    axis_init(&axis);

	world = game_world_new();
    player = mage_new(world);    
    world->player = player;
    world->level = level_new(world);

	follow();

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

		if (inputs.esc_pressed) break;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    level_free(world->level);
    world->level = NULL;
	player_free(player);
    world->player = NULL;
    game_world_free(world);

    // for debugging only
    _CrtDumpMemoryLeaks();

    return 0;
}

