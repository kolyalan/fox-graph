#ifndef MAIN_LEVEL_MAP_H
#define MAIN_LEVEL_MAP_H

#include "Image.h"
#include <cstring>
#include <string>

struct MapPoint {
    int x;
    int y;
};

enum TileType
{
    WALL_TOP_LEFT_TOP,
    WALL_TOP_LEFT_TOP_AMBIENT,
    WALL_TOP_LEFT_BOTTOM,
    WALL_TOP_RIGHT_TOP,
    WALL_TOP_RIGHT_TOP_AMBIENT,
    WALL_TOP_RIGHT_BOTTOM,
    WALL_TOP_TOP,
    WALL_TOP_TOP_AMBIENT,
    WALL_TOP_BOTTOM,
    WALL_BOTTOM_LEFT_TOP,
    WALL_BOTTOM_LEFT_BOTTOM,
    WALL_BOTTOM_RIGHT_TOP,
    WALL_BOTTOM_RIGHT_BOTTOM,
    WALL_BOTTOM_TOP,
    WALL_BOTTOM_BOTTOM,
    WALL_LEFT,
    WALL_RIGHT,
    WALL_FRONT_LEFT_TOP,
    WALL_FRONT_LEFT_BOTTOM,
    WALL_BACK_LEFT_TOP,
    WALL_BACK_LEFT_BOTTOM,
    WALL_FRONT_RIGHT_TOP,
    WALL_FRONT_RIGHT_BOTTOM,
    WALL_BACK_RIGHT_TOP,
    WALL_BACK_RIGHT_BOTTOM,
    PASSAGE_TOP_LEFT_TOP,
    PASSAGE_TOP_RIGHT_TOP,
    PASSAGE_TOP_LEFT_BOTTOM,
    PASSAGE_TOP_RIGHT_BOTTOM,
    FLOOR_0,
    FLOOR_1,
    FLOOR_2,
    FLOOR_3,
    FLOOR_4,
    FLOOR_5,
    FLOOR_6,
    FLOOR_7,
    FLOOR_8,
    FLOOR_9,
    FLOOR_10,
    FLOOR_11,
    EMPTY,
    EXIT,
    SPIKES_OFF,
    SPIKES_HALF,
    SPIKES_ON,
    NTYPES
};

static MapPoint tile_map[TileType::NTYPES] = {{3, 0},
                                              {7, 17},
                                              {3, 1},
                                              {5, 0},
                                              {9, 17},
                                              {5, 1},
                                              {4, 0},//WALL_TOP_TOP
                                              {8, 17},
                                              {4, 1},//WALL_TOP_BOTTOM
                                              {3, 2},
                                              {3, 3},
                                              {5, 2},
                                              {5, 3},//WALL_BOTTOM_RIGHT_BOTTOM
                                              {4, 2},
                                              {4, 3},//WALL_BOTTOM_BOTTOM
                                              {2, 1},
                                              {0, 1},//WALL_RIGHT
                                              {2, 2},
                                              {2, 3},
                                              {1, 6},
                                              {1, 7},
                                              {0, 2},
                                              {0, 3},
                                              {2, 6},
                                              {2, 7},
                                              {7, 0},// /PASSAGE
                                              {8, 0},//|
                                              {7, 1},//|
                                              {8, 1},// \PASSAGE
                                              {8, 5},//FLOOR0
                                              {9, 5},
                                              {10, 5},
                                              {11, 5},
                                              {12, 5},
                                              {13, 5},
                                              {8, 6},
                                              {9, 6},
                                              {10, 6},
                                              {11, 6},
                                              {12, 6},
                                              {13, 6},
                                              {1, 1}, 
                                              {1, 1},
                                              {5, 15},
                                              {4, 15},
                                              {3, 15}};


struct LevelMap
{
    LevelMap();
    LevelMap(const std::string &path);
    LevelMap(const LevelMap &other);
    LevelMap(LevelMap &&other);
    LevelMap& operator=(const LevelMap &other);
    LevelMap& operator=(LevelMap &&other);

    char * operator[] (int n) { return map + n * width;}

    bool PlayerCanMove(int x, int y);
    bool PlayerMustDie(int x, int y);
    bool PlayerMustWin(int x, int y);

    int reload(const std::string &path);
    void draw(Image &screen);
    int Width() const { return width; }
    int Height() const { return height; }
    MapPoint getStartPos() const { return startPos; };
    ~LevelMap();
private:
    int width = 0;
    int height = 0;
    MapPoint startPos = {-1, -1};
    char * map = nullptr;
    void drawEmpty(Image &screen);
    void drawFloor(Image &screen);
    void drawWalls(Image &screen);
    void drawTile(Image &screen, int x, int y, TileType type);
    bool isExternal(char c) { return (c == '.' || c == '@' || c == 'x' || c == 'T'); }
};

#endif // MAIN_LEVEL_MAP_H