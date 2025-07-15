static const float atlas_quad_dim = 1.0f / 16.0f;

static const int map_tile_cols = 20;
static const int map_tile_rows = 20;

static const int ui_tile_cols = 13;
static const int ui_tile_rows = 25;

static const int log_tile_cols = 20;
static const int log_tile_rows = 5;

static const float tile_dim = 24.0f;

#define COL_DARK_BG (Col_3f){0.1f,0.105f,0.1f}

static const int glyph_idx_vert_border = 186;

static const float ui_x_offset = map_tile_cols * tile_dim;

static const int glyph_idx_horiz_border = 205;

static const float log_y_offset = map_tile_rows * tile_dim;

static const Render_Glyph player_glyph = (Render_Glyph){'@', (Col_3f){0.9f,0.795f,0.8f}, COL_DARK_BG};


