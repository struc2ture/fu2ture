void generate_game_map(Game_State *state)
{
    if (state->map.tiles) game_map_free(&state->map);
    state->map = game_map_make(20, 20, TILE_GROUND);
    for (int x = 0; x < state->map.w; x++)
    {
        state->map.tiles[x] = TILE_WALL;
        state->map.tiles[state->map.w * (state->map.h - 1) + x] = TILE_WALL;
    }

    for (int y = 0; y < state->map.h; y++)
    {
        state->map.tiles[state->map.w * y] = TILE_WALL;
        state->map.tiles[state->map.w * y + state->map.w - 1] = TILE_WALL;
    }

    state->player.x = 10;
    state->player.y = 10;
}
