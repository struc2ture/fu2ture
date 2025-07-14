#pragma once

typedef struct {
    unsigned char idx;
    Col_3f fg, bg;
} Render_Glyph;

typedef struct {
    Render_Glyph *g;
    int w, h;
    float cell_dim;
} Glyph_Grid;

typedef struct {
    float x_min, x_max;
    float y_min, y_max;
} Quad;
