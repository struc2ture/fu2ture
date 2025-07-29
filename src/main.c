#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include "common.h"
#include "platform_types.h"

#include "future.h"

void on_init(Game_State *state, GLFWwindow *window, float window_w, float window_h, float window_px_w, float window_px_h, bool is_live_scene, GLuint fbo, int argc, char **argv)
{
}

void on_reload(Game_State *state)
{
}

void on_frame(Game_State *state, const Platform_Timing *t)
{
    glClearColor(0.7f, 0.7f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void on_platform_event(Game_State *state, const Platform_Event *e)
{
}

void on_destroy(Game_State *state)
{
}
