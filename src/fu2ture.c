#include "fu2ture.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "common.h"
#include "gl_glue.h"
#include "platform_types.h"

#include "consts.c"
#include "stuff.c"
#include "actions.c"

void on_init(Game_State *state, GLFWwindow *window, float window_w, float window_h, float window_px_w, float window_px_h, bool is_live_scene, GLuint fbo, int argc, char **argv)
{
    if (!is_live_scene) glfwSetWindowTitle(window, "fu2ture");

    state->window_dim.x = window_w;
    state->window_dim.y = window_h;
    state->framebuffer_dim.x = window_px_w;
    state->framebuffer_dim.y = window_px_h;

    const char *vs_src =
        "#version 330 core\n"
        "layout(location = 0) in vec2 aPos;\n"
        "layout(location = 1) in vec2 aTexCoord;\n"
        "layout(location = 2) in vec3 aFgColor;\n"
        "layout(location = 3) in vec3 aBgColor;\n"
        "out vec2 TexCoord;\n"
        "out vec3 FgColor;\n"
        "out vec3 BgColor;\n"
        "uniform mat4 u_mvp;\n"
        "void main() {\n"
        "  gl_Position = u_mvp * vec4(aPos, 0.0, 1.0);\n"
        "  TexCoord = aTexCoord;\n"
        "  FgColor = aFgColor;\n"
        "  BgColor = aBgColor;\n"
        "}\n";

    const char *fs_src =
        "#version 330 core\n"
        "in vec2 TexCoord;\n"
        "in vec3 FgColor;\n"
        "in vec3 BgColor;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D u_tex;\n"
        "void main() {\n"
        "  vec4 t = texture(u_tex, TexCoord);\n"
        "  vec4 fg_color = vec4(FgColor, 1.0) * t.g;\n"
        "  vec4 bg_color = vec4(BgColor, 1.0) * (1.0 - t.g);\n"
        "  FragColor = fg_color + bg_color;\n"
        "}\n";

    state->prog = gl_create_shader_program(vs_src, fs_src);

    state->vert_buffer = vert_buffer_make();

    glActiveTexture(GL_TEXTURE1);
    state->tex = gl_load_texture("/Users/struc/dev/jects/fu2ture/res/hack64.png", GL_NEAREST);

    generate_game_map(state);
}

void on_reload(Game_State *state)
{
    generate_game_map(state);
}

void on_frame(Game_State *state, const Platform_Timing *t)
{
    glViewport(0, 0, (GLsizei)state->framebuffer_dim.x, (GLsizei)state->framebuffer_dim.y);

    glClearColor(0.7f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(state->prog);

    Mat_4 proj = mat4_proj_ortho(0, state->window_dim.x, state->window_dim.y, 0, -1, 1);

    float w = 100;
    float h = 100;
    float x = state->window_dim.x * 0.5f - w * 0.5f;
    float y = state->window_dim.y * 0.5f - h * 0.5f;
    Mat_4 mvp = proj;
    glUniformMatrix4fv(glGetUniformLocation(state->prog, "u_mvp"), 1, GL_FALSE, mvp.m);

    glUniform1i(glGetUniformLocation(state->prog, "u_tex"), 1);

    glBindTexture(GL_TEXTURE_2D, state->tex.texture_id);

    vert_buffer_clear(state->vert_buffer);

    push_map_glyphs(state->vert_buffer, &state->map);
    push_ui_glyphs(state->vert_buffer);
    push_log_glyphs(state->vert_buffer);

    push_player_glyph(state->vert_buffer, state->player);

    vert_buffer_draw_call(state->vert_buffer);
}

void on_platform_event(Game_State *state, const Platform_Event *e)
{
    switch (e->kind)
    {
        case PLATFORM_EVENT_KEY:
        {
            if (e->key.action == GLFW_PRESS)
            {
                switch (e->key.key)
                {
                }
            }
            else if (e->key.action == GLFW_RELEASE)
            {
                switch (e->key.key)
                {
                }
            }
        } break;

        case PLATFORM_EVENT_MOUSE_BUTTON:
        {
        } break;

        case PLATFORM_EVENT_MOUSE_MOTION:
        {
        } break;

        case PLATFORM_EVENT_WINDOW_RESIZE:
        {
            state->window_dim.x = e->window_resize.logical_w;
            state->window_dim.y = e->window_resize.logical_h;
            state->framebuffer_dim.x = e->window_resize.px_w;
            state->framebuffer_dim.y = e->window_resize.px_h;
        } break;

        default: break;
    }
}

void on_destroy(Game_State *state)
{
    gl_delete_texture(&state->tex);
    vert_buffer_free(state->vert_buffer);
    glDeleteProgram(state->prog);
}
