void draw_glyph(Game_State *state, unsigned char glyph, Col_3f fg, Col_3f bg)
{
    int atlas_x = glyph % 16;
    int atlas_y = glyph / 16;
    float q_dim = 1.0f / 16.0f;
    float q_x_min = atlas_x * q_dim;
    float q_y_min = atlas_y * q_dim;
    float q_x_max = q_x_min + q_dim;
    float q_y_max = q_y_min + q_dim;
        
    Vert_Buffer vb = {0};
    vert_buffer_add_vert(&vb, (Vert){0, 1, q_x_min, q_y_max, fg, bg});
    vert_buffer_add_vert(&vb, (Vert){0, 0, q_x_min, q_y_min, fg, bg});
    vert_buffer_add_vert(&vb, (Vert){1, 0, q_x_max, q_y_min, fg, bg});
    vert_buffer_add_vert(&vb, (Vert){1, 1, q_x_max, q_y_max, fg, bg});

    glBindBuffer(GL_ARRAY_BUFFER, state->vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_buffer_max_size(), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vert_buffer_size(&vb), vb.verts);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}
