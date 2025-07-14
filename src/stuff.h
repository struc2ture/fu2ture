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
