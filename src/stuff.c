#include "stuff.h"

static const float atlas_quad_dim = 1.0f / 16.0f;

Quad render_glyph_get_quad(Render_Glyph g)
{
    int atlas_x = g.idx % 16;
    int atlas_y = g.idx / 16;
    float q_x_min = atlas_x * atlas_quad_dim;
    float q_y_min = atlas_y * atlas_quad_dim;
    float q_x_max = q_x_min + atlas_quad_dim;
    float q_y_max = q_y_min + atlas_quad_dim;
    return (Quad){q_x_min, q_x_max, q_y_min, q_y_max};
}

void render_glyph_add_verts(Vert_Buffer *vb, Render_Glyph g, float offset_x, float offset_y, float cell_dim)
{
    int index_base = vert_buffer_next_vert_index(vb);

    Quad q = render_glyph_get_quad(g);
    float d_x_min = offset_x;
    float d_x_max = d_x_min + cell_dim;
    float d_y_min = offset_y;
    float d_y_max = d_y_min + cell_dim;
    vert_buffer_add_vert(vb, (Vert){d_x_min, d_y_max, q.x_min, q.y_max, g.fg, g.bg});
    vert_buffer_add_vert(vb, (Vert){d_x_min, d_y_min, q.x_min, q.y_min, g.fg, g.bg});
    vert_buffer_add_vert(vb, (Vert){d_x_max, d_y_min, q.x_max, q.y_min, g.fg, g.bg});
    vert_buffer_add_vert(vb, (Vert){d_x_max, d_y_max, q.x_max, q.y_max, g.fg, g.bg});

    int indices[] = {0,2,1, 0,3,2};
    vert_buffer_add_indices(vb, index_base, indices, 6);
}

void draw_glyph(Vert_Buffer *vb, Render_Glyph g, Rect dest)
{
    vert_buffer_clear(vb);

    render_glyph_add_verts(vb, g, dest.x, dest.y, dest.w);

    vert_buffer_draw_call(vb);
}

void render_glyph_push(Vert_Buffer *vb, Render_Glyph glyph, int col, int row, Grid_Spec grid)
{
    float x = grid.offset_x + (float)col * grid.tile_dim;
    float y = grid.offset_y + (float)row * grid.tile_dim;
    render_glyph_add_verts(vb, glyph, x, y, grid.tile_dim);
}

static const int map_tile_cols = 20;
static const int map_tile_rows = 20;

static const int ui_tile_cols = 13;
static const int ui_tile_rows = 25;

static const int log_tile_cols = 20;
static const int log_tile_rows = 5;

static const float tile_dim = 24.0f;

void push_map_glyphs(Vert_Buffer *vb)
{
    Grid_Spec grid_spec = {
        .tile_dim = tile_dim
    };

    for (int row = 0; row < map_tile_rows; row++)
    {
        for (int col = 0; col < map_tile_cols; col++)
        {
            render_glyph_push(vb,
                              (Render_Glyph){'.', (Col_3f){0.3f,0.295f,0.3f}, (Col_3f){0.1f,0.105f,0.1f}},
                              col, row,
                              grid_spec);
        }
    }
}

static const int glyph_idx_vert_border = 186;

static const float ui_x_offset = map_tile_cols * tile_dim;

void push_ui_glyphs(Vert_Buffer *vb)
{
    Grid_Spec grid_spec = {
        .tile_dim = tile_dim,
        .offset_x = ui_x_offset
    };

    for (int row = 0; row < ui_tile_rows; row++)
    {
        for (int col = 0; col < ui_tile_cols; col++)
        {
            Render_Glyph g;
            if (col == 0)
            {
                g = (Render_Glyph){glyph_idx_vert_border, (Col_3f){0.3f,0.295f,0.3f}, (Col_3f){0.1f,0.105f,0.1f}};
            }
            else
            {
                g = (Render_Glyph){'U', (Col_3f){0.3f,0.295f,0.3f}, (Col_3f){0.1f,0.105f,0.1f}};
            }
            render_glyph_push(vb,
                              g,
                              col, row,
                              grid_spec);
        }
    }
}

static const int glyph_idx_horiz_border = 205;

static const float log_y_offset = map_tile_rows * tile_dim;

void push_log_glyphs(Vert_Buffer *vb)
{
    Grid_Spec grid_spec = {
        .tile_dim = tile_dim,
        .offset_y = log_y_offset
    };

    for (int row = 0; row < log_tile_rows; row++)
    {
        for (int col = 0; col < log_tile_cols; col++)
        {
            Render_Glyph g;
            if (row == 0)
            {
                g = (Render_Glyph){glyph_idx_horiz_border, (Col_3f){0.3f,0.295f,0.3f}, (Col_3f){0.1f,0.105f,0.1f}};
            }
            else
            {
                g = (Render_Glyph){'L', (Col_3f){0.3f,0.295f,0.3f}, (Col_3f){0.1f,0.105f,0.1f}};
            }
            render_glyph_push(vb,
                              g,
                              col, row,
                              grid_spec);
        }
    }
}
