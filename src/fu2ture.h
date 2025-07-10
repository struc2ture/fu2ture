#pragma once

#include "common.h"

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include "platform_types.h"

typedef struct {
    Vec_2 window_dim;
    Vec_2 framebuffer_dim;
    GLuint prog;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint tex;
} Game_State;

void on_init(Game_State *state, GLFWwindow *window, float window_w, float window_h, float window_px_w, float window_px_h, bool is_live_scene, GLuint fbo, int argc, char **argv);
void on_reload(Game_State *state);
void on_frame(Game_State *state, const Platform_Timing *t);
void on_platform_event(Game_State *state, const Platform_Event *e);
void on_destroy(Game_State *state);
