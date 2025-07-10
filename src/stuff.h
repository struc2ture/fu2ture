#pragma once

typedef struct {
    unsigned char idx;
    Col_3f fg, bg;
} Map_Glyph;

typedef struct {
    Map_Glyph *g;
    int w, h;
} Map_Grid_Layer;

typedef struct {
    float x_min, x_max;
    float y_min, y_max;
} Quad;
