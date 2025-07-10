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
        "layout(location = 2) in vec3 aColor;\n"
        "out vec2 TexCoord;\n"
        "out vec3 Color;\n"
        "uniform mat4 u_mvp;\n"
        "void main() {\n"
        "  gl_Position = u_mvp * vec4(aPos, 0.0, 1.0);\n"
        "  TexCoord = aTexCoord;\n"
        "  Color = aColor;\n"
        "}\n";

    const char *fs_src =
        "#version 330 core\n"
        "in vec2 TexCoord;\n"
        "in vec3 Color;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D u_tex;\n"
        "void main() {\n"
        "  vec4 t = texture(u_tex, TexCoord);\n"
        "  vec4 tex_color = vec4(t.r, t.r, t.r, t.g);\n"
        "  FragColor = vec4(Color, 1.0) * tex_color;\n"
        "}\n";

    state->prog = gl_create_shader_program(vs_src, fs_src);

    float tri_verts[] = {
         0, 1,  0, 1,  1, 1, 1,
         0, 0,  0, 0,  1, 0, 0,
         1, 0,  1, 0,  0, 1, 0,
         1, 1,  1, 1,  0, 0, 1,
    };

    unsigned char tri_indices[] = {
        0,2,1, 0,3,2
    };

    glGenVertexArrays(1, &state->vao);
    glGenBuffers(1, &state->vbo);
    glGenBuffers(1, &state->ebo);

    glBindVertexArray(state->vao);
    glBindBuffer(GL_ARRAY_BUFFER, state->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri_verts), tri_verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tri_indices), tri_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    
    int tex_w, tex_h, tex_ch;
    unsigned char *tex_data = stbi_load("res/hack64.png", &tex_w, &tex_h, &tex_ch, 0);
    GLenum tex_format = (tex_ch == 3 ? GL_RGB : GL_RGBA);
    //if (tex_ch == 2) tex_format = GL_RG;
    
    printf("TEXTURE: %d, %d, %d\n", tex_w, tex_h, tex_ch);
    
    glGenTextures(1, &state->tex);
    glBindTexture(GL_TEXTURE_2D, state->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, tex_w, tex_h, 0, GL_RG, GL_UNSIGNED_BYTE, tex_data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(tex_data);
}

void on_reload(Game_State *state)
{
    (void)state;
}

void on_frame(Game_State *state, const Platform_Timing *t)
{
    glViewport(0, 0, (GLsizei)state->framebuffer_dim.x, (GLsizei)state->framebuffer_dim.y);
    
    glClearColor(0.7f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(state->prog);
    
    Mat_4 proj = mat4_proj_ortho(0, state->window_dim.x, state->window_dim.y, 0, -1, 1);

    float w = 600;
    float h = 600;
    float x = state->window_dim.x * 0.5f - w * 0.5f;
    float y = state->window_dim.y * 0.5f - h * 0.5f;
    Mat_4 model = mat4_mul(mat4_translate(x, y, 0), mat4_scale(w, h, 0));
    Mat_4 mvp = mat4_mul(proj, model);
    glUniformMatrix4fv(glGetUniformLocation(state->prog, "u_mvp"), 1, GL_FALSE, mvp.m);
    
    glUniform1i(glGetUniformLocation(state->prog, "u_tex"), 1);
    
    glBindVertexArray(state->vao);
    
    glBindTexture(GL_TEXTURE_2D, state->tex);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
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
    glDeleteTextures(1, &state->tex);
    glDeleteBuffers(1, &state->vbo);
    glDeleteBuffers(1, &state->ebo);
    glDeleteVertexArrays(1, &state->vao);
    glDeleteProgram(state->prog);
}
