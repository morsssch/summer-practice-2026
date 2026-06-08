#include "screen_01.h"
#include "level_data.h"

const Room SCREEN_01 = []() {
    Room r = {};
    r.w = 60;
    r.h = 34;

    // потолок
    for (int x = 0; x < 60; x++)
        r.tiles[0][x] = T_DARK_TOP;

    // левая стена
    for (int y = 1; y < 32; y++)
        r.tiles[y][0] = r.tiles[y][1] = T_DARK;

    // правая стена
    for (int y = 1; y < 32; y++)
        r.tiles[y][58] = r.tiles[y][59] = T_DARK;

    // пол
    for (int x = 0; x < 60; x++) {
        r.tiles[32][x] = T_GRASS_TOP;
        r.tiles[33][x] = T_GROUND;
    }

    // платформы
    auto plat = [&](int row, int x1, int x2) {
        for (int x = x1; x <= x2; x++) r.tiles[row][x] = T_PLATFORM;
    };

    plat(27,  5, 10); // низ-лево
    plat(27, 48, 53); // низ-право
    plat(25, 21, 25); // центр-лево (переходная)
    plat(25, 34, 38); // центр-право (переходная)
    plat(22, 13, 18); // середина-лево
    plat(22, 40, 45); // середина-право
    plat(17, 26, 32); // вверх-центр

    // враги
    r.tiles[31][15] = T_ENEMY_LIGHT; // гриб: пол, левая зона
    r.tiles[31][44] = T_ENEMY_LIGHT; // гриб: пол, правая зона
    r.tiles[21][15] = T_ENEMY_LIGHT; // гриб: платформа середина-лево
    r.tiles[21][42] = T_ENEMY_LIGHT; // гриб: платформа середина-право

    r.tiles[26][ 7] = T_ENEMY_HEAVY; // слизень: низ-лево платформа
    r.tiles[26][51] = T_ENEMY_HEAVY; // слизень: низ-право платформа
    r.tiles[16][29] = T_ENEMY_HEAVY; // слизень: верх-центр платформа


    r.tiles[31][ 4] = T_TORCH;
    r.tiles[31][55] = T_TORCH;
    r.tiles[26][ 5] = T_TORCH;
    r.tiles[26][53] = T_TORCH;
    r.tiles[21][13] = T_TORCH;
    r.tiles[21][45] = T_TORCH;
    r.tiles[16][27] = T_TORCH;

    r.tiles[31][ 3] = T_SPIKE;
    r.tiles[31][56] = T_SPIKE;
    r.tiles[31][57] = T_SPIKE;

    r.tiles[ 1][29] = T_SPIKE_CEIL;
    r.tiles[ 1][30] = T_SPIKE_CEIL;
    r.tiles[ 1][31] = T_SPIKE_CEIL;

    r.tiles[31][30] = T_DOOR_0;
    r.tiles[31][23] = T_PLATE_0;

    return r;
}();
