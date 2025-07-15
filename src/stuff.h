#pragma once

typedef struct {
    unsigned char idx;
    Col_3f fg, bg;
} Render_Glyph;

typedef struct {
    float tile_dim;
    float offset_x;
    float offset_y;
} Grid_Spec;

typedef struct {
    float x_min, x_max;
    float y_min, y_max;
} Quad;

typedef enum {
    TILE_NONE,
    TILE_GROUND,
    TILE_WALL
} Tile_Kind;

typedef struct {
    Tile_Kind *tiles;
    int w, h;
} Game_Map;

typedef struct {
    int x, y;
} Player;
