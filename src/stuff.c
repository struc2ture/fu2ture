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

Glyph_Grid glyph_grid_make(int w, int h, Render_Glyph base_glyph, float cell_dim)
{
    Glyph_Grid grid;
    grid.w = w;
    grid.h = h;
    grid.cell_dim = cell_dim;
    grid.g = malloc(grid.w * grid.h * sizeof(grid.g[0]));
    for (int i = 0; i < grid.w * grid.h; i++)
    {
        grid.g[i] = base_glyph;
    }
    return grid;
}

void glyph_grid_free(Glyph_Grid *grid)
{
    free(grid->g);
    grid->g = NULL;
    grid->w = 0;
    grid->h = 0;
}

void draw_glyph_grid(Vert_Buffer *vb, const Glyph_Grid *grid, float offset_x, float offset_y)
{
    vert_buffer_clear(vb);

    for (int row = 0; row < grid->h; row++)
    {
        for (int col = 0; col < grid->w; col++)
        {
            float x = offset_x + (float)col * grid->cell_dim;
            float y = offset_y + (float)row * grid->cell_dim;
            render_glyph_add_verts(vb, grid->g[row * grid->w + col], x, y, grid->cell_dim);
        }
    }

    vert_buffer_draw_call(vb);
}
