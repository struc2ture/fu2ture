#include "stuff.h"

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

Render_Glyph get_glyph_for_tile(Tile_Kind kind)
{
    switch (kind)
    {
        case TILE_NONE: return (Render_Glyph){'!', (Col_3f){1,0,0}, (Col_3f){1,1,0}};
        case TILE_GROUND: return (Render_Glyph){'.', (Col_3f){0.3f,0.295f,0.3f}, COL_DARK_BG};
        case TILE_WALL: return (Render_Glyph){'#', (Col_3f){0.6f,0.595f,0.6f}, COL_DARK_BG};
    }
}

void push_map_glyphs(Vert_Buffer *vb, const Game_Map *map)
{
    Grid_Spec grid_spec = {
        .tile_dim = tile_dim
    };

    for (int row = 0; row < map_tile_rows; row++)
    {
        for (int col = 0; col < map_tile_cols; col++)
        {
            render_glyph_push(vb,
                              get_glyph_for_tile(map->tiles[row * map->w + col]),
                              col, row,
                              grid_spec);
        }
    }
}

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

Game_Map game_map_make(int w, int h, Tile_Kind base_tile)
{
    Game_Map map;
    map.tiles = malloc(w * h * sizeof(map.tiles[0]));
    for (int i = 0; i < w * h; i++)
    {
        map.tiles[i] = base_tile;
    }
    map.w = w;
    map.h = h;
    return map;
}

void game_map_free(Game_Map *map)
{
    free(map->tiles);
    map->w = 0;
    map->h = 0;
}

void push_player_glyph(Vert_Buffer *vb, Player player)
{
    Grid_Spec grid_spec = {
        .tile_dim = tile_dim
    };

    render_glyph_push(vb,
                      player_glyph,
                      player.x, player.y,
                      grid_spec);
}
