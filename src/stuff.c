#include "stuff.h"

static const float atlas_quad_dim = 1.0f / 16.0f;

Quad map_glyph_get_quad(Map_Glyph g)
{
    int atlas_x = g.idx % 16;
    int atlas_y = g.idx / 16;
    float q_x_min = atlas_x * atlas_quad_dim;
    float q_y_min = atlas_y * atlas_quad_dim;
    float q_x_max = q_x_min + atlas_quad_dim;
    float q_y_max = q_y_min + atlas_quad_dim;
    return (Quad){q_x_min, q_x_max, q_y_min, q_y_max};
}

void map_glyph_add_verts(Vert_Buffer *vb, Map_Glyph g, float offset_x, float offset_y, float cell_dim)
{
    int index_base = vert_buffer_next_vert_index(vb);

    Quad q = map_glyph_get_quad(g);
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

void draw_map_glyph(Vert_Buffer *vb, Map_Glyph g, Rect dest)
{
    vert_buffer_clear(vb);

    map_glyph_add_verts(vb, g, dest.x, dest.y, dest.w);

    vert_buffer_draw_call(vb);
}

Map_Grid_Layer map_grid_layer_make(int w, int h, Map_Glyph base_glyph)
{
    Map_Grid_Layer layer;
    layer.w = w;
    layer.h = h;
    layer.g = malloc(layer.w * layer.h * sizeof(layer.g[0]));
    for (int i = 0; i < layer.w * layer.h; i++)
    {
        layer.g[i] = base_glyph;
    }
    return layer;
}

void map_grid_layer_free(Map_Grid_Layer *layer)
{
    free(layer->g);
    layer->g = NULL;
    layer->w = 0;
    layer->h = 0;
}

void draw_map_grid_layer(Vert_Buffer *vb, const Map_Grid_Layer *layer, float offset_x, float offset_y, float cell_dim)
{
    vert_buffer_clear(vb);

    for (int row = 0; row < layer->h; row++)
    {
        for (int col = 0; col < layer->w; col++)
        {
            float x = offset_x + (float)row * cell_dim;
            float y = offset_y + (float)col * cell_dim;
            map_glyph_add_verts(vb, layer->g[row * layer->w + col], x, y, cell_dim);
        }
    }
    
    vert_buffer_draw_call(vb);
}
