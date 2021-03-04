#include "LevelMap.h"

#include <iostream>

LevelMap::LevelMap() {
    map = nullptr;
}

LevelMap::LevelMap(const std::string &path) {
    if (reload(path)) throw -1;
}

int LevelMap::reload(const std::string &path) {
    if (map != nullptr) {
        delete[] map;
        map = nullptr;
    }
    startPos = {-1, -1};
    FILE * f = fopen(path.c_str(), "r");
    if (!f) {
        std::cout << "Unable to open level map file: " << path << std::endl;
        return -1;
    }
    fscanf(f, "%dx%d\n", &width, &height);
    char * tmpmap = new char[width*height];
    int c;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if ((c = fgetc(f)) != EOF) {
                tmpmap[i * width + j] = c;
                if (c == '@') {
                    if (startPos.x != -1 || startPos.y != -1) {
                        std::cout << "Incorrect file format. Several start positions" << std::endl;
                        return -1;
                    }
                    startPos = {j, i+1};
                }
            } else {
                std::cout << "Incorrect file format. line " << i + 2 << ", col " << j + 1 << std::endl;
                delete[] tmpmap;
                return -1;
            }
        }
        if ((c = fgetc(f)) == EOF) {
            std::cout << "Incorrect file format. line " << i + 2 << ", col " << width + 1 << std::endl;
            delete[] tmpmap;
            return -1;
        }
        if (c != '\n') {
            std::cout << "Unexpected line terminator. line " << i + 2 << ", col " << width + 1 << std::endl;
            delete[] tmpmap;
            return -1;
        }
    }
    map = tmpmap;
    /* //DEBUG
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << map[i*width + j] << ' ';
        }
        std::cout << std::endl;
    }*/
    return 0;
}

LevelMap::LevelMap(const LevelMap &other) {
    width = other.width;
    height = other.height;
    startPos = other.startPos;
    if (other.map == nullptr) {
        map = nullptr;
    } else {
        map = new char[width * height];
        memcpy(map, other.map, width * height);
    }
}

LevelMap::LevelMap(LevelMap &&other) {
    width = other.width;
    height = other.height;
    startPos = other.startPos;
    map = other.map;
    other.map = nullptr;
}

LevelMap& LevelMap::operator=(const LevelMap &other) {
    if (this == &other) {
        return *this;
    }
    width = other.width;
    height = other.height;
    startPos = other.startPos;
    if (map != nullptr) {
        delete [] map;
    }
    if (other.map == nullptr) {
        map = nullptr;
    } else {
        map = new char[width * height];
        memcpy(map, other.map, width * height);
    }
    return *this;
}

LevelMap& LevelMap::operator=(LevelMap &&other) {
    if (this == &other) {
        return *this;
    }
    width = other.width;
    height = other.height;
    startPos = other.startPos;
    if (map != nullptr) {
        delete [] map;
    }
    if (other.map == nullptr) {
        map = nullptr;
    } else {
        map = other.map;
        other.map = nullptr;
    }
    return *this;
}

LevelMap::~LevelMap() {
    if (map != nullptr) {
        delete [] map;
    }
}

//accepts player's coordinates
bool LevelMap::PlayerCanMove(int x, int y) {
    MapPoint topCenter    = {(x+2*tileSize)/(2*tileSize)-1, (y + tileSize / 2) / (2*tileSize)};
    MapPoint bottomCenter = {(x+2*tileSize)/(2*tileSize)-1, (y + tileSize + 5) / (2*tileSize)};
    MapPoint topRight     = {(x+3*tileSize + 12)/(2*tileSize)-1, (y + tileSize / 2) / (2*tileSize)};
    MapPoint topLeft      = {(x+1*tileSize - 12)/(2*tileSize)-1, (y + tileSize / 2) / (2*tileSize)};
    if (y < 2*tileSize || topCenter.y < 1 || topCenter.y > height || topCenter.x < 0 || topCenter.x >= width) {
        return false;
    }
    if (map[(topCenter.y - 1) * width + topCenter.x] == '#') {
        return false;
    }
    if (map[(bottomCenter.y -1) *width + bottomCenter.x] == '#') {
        return false;
    }
    if ((map[(topCenter.y - 1) * width + topCenter.x] == 'x' && 
        (map[(topCenter.y) * width + topCenter.x] == 'x' ||
        (topLeft.x  >=    0 && map[(topLeft.y - 1) * width + topLeft.x]  == '#') ||
        (topRight.x < width && map[(topRight.y -1) * width + topRight.x] == '#')))) {
        return false;
    }
    return true;
}

//accepts player's coordinates
bool LevelMap::PlayerMustDie(int x, int y) {
    MapPoint topCenter    = {(x+2*tileSize)/(2*tileSize)-1, (y + tileSize / 2) / (2*tileSize)};
    MapPoint bottomCenter = {(x+2*tileSize)/(2*tileSize)-1, (y + tileSize-8) / (2*tileSize)};
    MapPoint topRight     = {(x+3*tileSize + 12)/(2*tileSize)-1, (y + tileSize / 2) / (2*tileSize)};
    MapPoint topLeft      = {(x+1*tileSize - 12)/(2*tileSize)-1, (y + tileSize / 2) / (2*tileSize)};
    if (map[(bottomCenter.y -1) *width + bottomCenter.x] == ' ' ||
        map[(bottomCenter.y -1) *width + bottomCenter.x] == 'T') {
        return true;
    }
    return false;
}

//accepts player's coordinates
bool LevelMap::PlayerMustWin(int x, int y) {
    MapPoint Center = {(x+2*tileSize)/(2*tileSize)-1, (y + tileSize-8) / (2*tileSize)};
    MapPoint Up           = {(x+2*tileSize)/(2*tileSize)-1, (y + tileSize + 24) / (2*tileSize)};
    MapPoint Left         = {(x+3*tileSize + 12)/(2*tileSize)-1, (y + tileSize / 2) / (2*tileSize)};
    MapPoint Right        = {(x+1*tileSize - 12)/(2*tileSize)-1, (y + tileSize / 2) / (2*tileSize)};
    if (Center.y == 1 && map[(Up.y - 1) * width + Up.x] == 'x') {
        return true;
    }
    if (Center.x == 0 && map[(Left.y - 1) * width + Left.x] == 'x') {
        return true;
    }
    if (Center.x == width-1 && map[(Right.y - 1) * width + Right.x] == 'x') {
        return true;
    }
    return false;
}

void LevelMap::draw(Image &screen) {
    Pixel black = {.r=0, .g=0, .b=0, .a=255};
    if (map == nullptr) {
        for (int i = 0; i < screen.Width()*screen.Height(); i++) {
            screen.Data()[i] = black;
        }
        return;
    }
    if (screen.Width() < width*tileSize * 2 || screen.Height() < (height+1) * tileSize * 2) {
        std::cout << "Cannot draw level map: screen size is not enough" << std::endl;
        return;
    }

    drawEmpty(screen);
    drawFloor(screen);
    drawWalls(screen);
}

void LevelMap::drawEmpty(Image &screen) {
    for (int y = 0; y <= height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || map[(y-1)*width+x] == ' ') {
                drawTile(screen, x, y, TileType::EMPTY);
            }
        }
    }
}

void LevelMap::drawFloor(Image &screen) {
    for (int y = 1; y <= height; y++) {
        for (int x = 0; x < width; x++) {
            if ((map[(y-1)*width+x] == '.') || 
                (map[(y-1)*width+x] == '#') ||
                (map[(y-1)*width+x] == '@') ||
                (map[(y-1)*width+x] == 'x') ||
                (map[(y-1)*width+x] == 'T')) {
                drawTile(screen, x, y, (TileType)((int)(TileType::FLOOR_0) + rand() % 12));
            }
            if (map[(y-1)*width+x] == 'T') {
                drawTile(screen, x, y, TileType::SPIKES_ON);
            }
        }
    }
}

void LevelMap::drawWalls(Image &screen) {
    for (int y = 1; y <= height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[(y-1)*width+x] == '#') {
                if (y == 1) {
                    if (x == 0 || 
                        (map[y*width+x] == '#' && (map[y*width+x-1] == '#' || 
                                                   map[y*width+x-1] == ' '))) {
                        drawTile(screen, x, y-1, TileType::WALL_TOP_LEFT_TOP);
                        drawTile(screen, x,   y, TileType::WALL_TOP_LEFT_BOTTOM);
                    } else if (x == width-1 || 
                                (map[y*width+x] == '#' && (map[y*width+x+1] == '#' || 
                                                           map[y*width+x+1] == ' '))) {
                        drawTile(screen, x, y-1, TileType::WALL_TOP_RIGHT_TOP);
                        drawTile(screen, x, y, TileType::WALL_TOP_RIGHT_BOTTOM);
                    } else {
                        drawTile(screen, x, y-1, TileType::WALL_TOP_TOP);
                        drawTile(screen, x,   y, TileType::WALL_TOP_BOTTOM);
                    }
                } else if (y == height) {
                    if (map[(y-2)*width+x] != 'x' && x == 0 || 
                        (map[(y-2)*width+x] == '#' && (map[(y-2)*width+x-1] == '#' || 
                                                       map[(y-2)*width+x-1] == ' '))) {
                        drawTile(screen, x, y-1, TileType::WALL_BOTTOM_LEFT_TOP);
                        drawTile(screen, x,   y, TileType::WALL_BOTTOM_LEFT_BOTTOM);
                    } else if (map[(y-2)*width+x] != 'x' && x == width-1 || 
                        (map[(y-2)*width+x] == '#' && (map[(y-2)*width+x+1] == '#' || 
                                                       map[(y-2)*width+x+1] == ' '))) {
                        drawTile(screen, x, y-1, TileType::WALL_BOTTOM_RIGHT_TOP);
                        drawTile(screen, x,   y, TileType::WALL_BOTTOM_RIGHT_BOTTOM);
                    } else {
                        drawTile(screen, x, y-1, TileType::WALL_BOTTOM_TOP);
                        drawTile(screen, x,   y, TileType::WALL_BOTTOM_BOTTOM);
                    }
                } else { //in the middle by height
                    if (x == 0) {
                        if (map[(y-2)*width+x] == 'x') {
                            drawTile(screen, x, y, WALL_BACK_LEFT_BOTTOM);
                        } else if (map[(y-1)*width+x+1] == '#' && (map[y*width+x+1] == '#' || 
                                                                   map[y*width+x+1] == ' ')) {
                            drawTile(screen, x, y-1, TileType::WALL_BOTTOM_LEFT_TOP);
                            drawTile(screen, x,   y, TileType::WALL_BOTTOM_LEFT_BOTTOM);
                        } else if (map[(y-1)*width+x+1] == '#' && (map[(y-2)*width+x+1] == '#' || 
                                                                   map[(y-2)*width+x+1] == ' ')) {
                            drawTile(screen, x, y-1, TileType::WALL_TOP_LEFT_TOP_AMBIENT);
                            drawTile(screen, x,   y, TileType::WALL_TOP_LEFT_BOTTOM);
                        } else {
                            drawTile(screen, x,   y, TileType::WALL_LEFT);
                        }
                    } else if (x == width-1) {
                        if (map[(y-2)*width+x] == 'x') {
                            drawTile(screen, x, y, WALL_BACK_RIGHT_BOTTOM);
                        } else if (map[(y-1)*width+x-1] == '#' && (map[y*width+x-1] == '#' || 
                                                                   map[y*width+x-1] == ' ')) {
                            drawTile(screen, x, y-1, TileType::WALL_BOTTOM_RIGHT_TOP);
                            drawTile(screen, x,   y, TileType::WALL_BOTTOM_RIGHT_BOTTOM);
                        } else if (map[(y-1)*width+x-1] == '#' && (map[(y-2)*width+x+1] == '#' || 
                                                                   map[(y-2)*width+x+1] == ' ')) {
                            drawTile(screen, x, y-1, TileType::WALL_TOP_RIGHT_TOP_AMBIENT);
                            drawTile(screen, x,   y, TileType::WALL_TOP_RIGHT_BOTTOM);
                        } else {
                            drawTile(screen, x, y, TileType::WALL_RIGHT);
                        }
                    } else {//middle by height and width
                        if (map[(y-2)*width+x] == '#') {
                            if (map[y*width+x] == '#') { // | line
                                if (map[(y-1)*width+x+1] == '#') { // |-
                                    if (map[(y-1)*width+x-1] == '#') { // -|-
                                        if (isExternal(map[(y-2)*width+x+1])) { 
                                            drawTile(screen, x, y-1, TileType::WALL_BOTTOM_LEFT_TOP);
                                            drawTile(screen, x,   y, TileType::WALL_BOTTOM_LEFT_BOTTOM);
                                        } else if (isExternal(map[(y-2)*width+x-1])) {
                                            drawTile(screen, x, y-1, TileType::WALL_BOTTOM_RIGHT_TOP);
                                            drawTile(screen, x,   y, TileType::WALL_BOTTOM_RIGHT_BOTTOM);
                                        } else if (isExternal(map[y*width+x-1])) {
                                            drawTile(screen, x, y-1, TileType::WALL_TOP_RIGHT_TOP_AMBIENT);
                                            drawTile(screen, x,   y, TileType::WALL_TOP_RIGHT_BOTTOM);
                                        } else if (isExternal(map[y*width+x+1])) {
                                            drawTile(screen, x, y-1, TileType::WALL_TOP_LEFT_TOP_AMBIENT);
                                            drawTile(screen, x,   y, TileType::WALL_TOP_LEFT_BOTTOM);
                                        } else {
                                            std::cout << "Incorrect level map. Cross of # without internal angles." << std::endl;
                                        }
                                    } else { // exactly |-
                                        if (isExternal(map[(y-2)*width+x+1])) {
                                            drawTile(screen, x, y-1, TileType::WALL_BOTTOM_LEFT_TOP);
                                            drawTile(screen, x,   y, TileType::WALL_BOTTOM_LEFT_BOTTOM);
                                        } else if (isExternal(map[y*width+x+1])) {
                                            drawTile(screen, x, y-1, TileType::WALL_TOP_LEFT_TOP_AMBIENT);
                                            drawTile(screen, x,   y, TileType::WALL_TOP_LEFT_BOTTOM);
                                        } else {
                                            drawTile(screen, x, y, TileType::WALL_RIGHT);
                                        }
                                    }
                                } else { // no # to right
                                    if (map[(y-1)*width+x-1] == '#') { // exactly -|
                                        if (isExternal(map[(y-2)*width+x-1])) {
                                            drawTile(screen, x, y-1, TileType::WALL_BOTTOM_RIGHT_TOP);
                                            drawTile(screen, x,   y, TileType::WALL_BOTTOM_RIGHT_BOTTOM);
                                        } else if (isExternal(map[y*width+x-1])) {
                                            drawTile(screen, x, y-1, TileType::WALL_TOP_RIGHT_TOP_AMBIENT);
                                            drawTile(screen, x,   y, TileType::WALL_TOP_RIGHT_BOTTOM);
                                        } else {
                                            drawTile(screen, x, y, TileType::WALL_LEFT);
                                        }
                                    } else { //exactly |
                                        if (isExternal(map[(y-1)*width+x-1]) || isExternal(map[y*width+x-1]) || isExternal(map[(y-2)*width+x-1])) {
                                            drawTile(screen, x, y, TileType::WALL_RIGHT);
                                        } else if (isExternal(map[(y-1)*width+x+1]) || isExternal(map[y*width+x+1]) || isExternal(map[(y-2)*width+x+1])) {
                                            drawTile(screen, x, y, TileType::WALL_LEFT);
                                        } else {
                                            std::cout << "Incorrect level map. Internal vertical wall in empty area." << std::endl;
                                        }
                                    }
                                }
                            } else if (map[(y-1)*width+x+1] == '#') {
                                if (map[(y-1)*width+x-1] == '#') {// _|_ 
                                    if (isExternal(map[(y-2)*width+x+1])) { 
                                        drawTile(screen, x, y-1, TileType::WALL_BOTTOM_LEFT_TOP);
                                        drawTile(screen, x,   y, TileType::WALL_BOTTOM_LEFT_BOTTOM);
                                    } else if (isExternal(map[(y-2)*width+x-1])) {
                                        drawTile(screen, x, y-1, TileType::WALL_BOTTOM_RIGHT_TOP);
                                        drawTile(screen, x,   y, TileType::WALL_BOTTOM_RIGHT_BOTTOM);
                                    } else { //#_#
                                        drawTile(screen, x, y-1, TileType::WALL_TOP_TOP_AMBIENT); //TODO
                                        drawTile(screen, x,   y, TileType::WALL_TOP_BOTTOM);
                                    }
                                } else if (isExternal(map[(y-2)*width+x+1])) { //internal L angle 
                                    drawTile(screen, x, y-1, TileType::WALL_BOTTOM_LEFT_TOP);
                                    drawTile(screen, x,   y, TileType::WALL_BOTTOM_LEFT_BOTTOM);
                                } else { // external L angle
                                    drawTile(screen, x, y-1, TileType::WALL_FRONT_RIGHT_TOP);
                                    drawTile(screen, x,   y, TileType::WALL_FRONT_RIGHT_BOTTOM);
                                }
                            } else if (map[(y-1)*width+x-1] == '#') {//_| angle
                                if (isExternal(map[(y-2)*width+x-1])) {//internal
                                    drawTile(screen, x, y-1, TileType::WALL_BOTTOM_RIGHT_TOP);
                                    drawTile(screen, x,   y, TileType::WALL_BOTTOM_RIGHT_BOTTOM);
                                } else {//external
                                    drawTile(screen, x, y-1, TileType::WALL_FRONT_LEFT_TOP);
                                    drawTile(screen, x,   y, TileType::WALL_FRONT_LEFT_BOTTOM);
                                }
                            } else {
                                std::cout << "Incorrect level map. # under # without other walls nearby." << std::endl;
                            }
                        } else if (map[(y-1)*width+x-1] == '#') { // no # above, is left
                            if (map[(y-1)*width+x+1] == '#') { // -- line
                                if (map[y*width+x] == '#') { // ^|^
                                    if (isExternal(map[y*width+x-1])) {
                                        drawTile(screen, x, y-1, TileType::WALL_TOP_RIGHT_TOP);
                                        drawTile(screen, x,   y, TileType::WALL_TOP_RIGHT_BOTTOM);
                                    } else if (isExternal(map[y*width+x+1])) {
                                        drawTile(screen, x, y-1, TileType::WALL_TOP_LEFT_TOP_AMBIENT);
                                        drawTile(screen, x,   y, TileType::WALL_TOP_LEFT_BOTTOM);
                                    } else {
                                        drawTile(screen, x, y-1, TileType::WALL_BOTTOM_TOP);
                                        drawTile(screen, x,   y, TileType::WALL_BOTTOM_BOTTOM);
                                    }
                                } else { // exactly --
                                    if (isExternal(map[(y-2)*width+x]) || isExternal(map[(y-2)*width+x-1]) || isExternal(map[(y-2)*width+x+1])) {
                                        drawTile(screen, x, y-1, TileType::WALL_BOTTOM_TOP);
                                        drawTile(screen, x,   y, TileType::WALL_BOTTOM_BOTTOM);
                                    } else if (isExternal(map[y*width+x]) || isExternal(map[y*width+x-1]) || isExternal(map[y*width+x+1])) {
                                        drawTile(screen, x, y-1, TileType::WALL_TOP_TOP);
                                        drawTile(screen, x,   y, TileType::WALL_TOP_BOTTOM);
                                    } else {
                                        std::cout << "Incorrect level map. Internal horizontal wall in emply area." << std::endl;
                                    }
                                }
                            } else if (map[y*width+x] == '#') { // ^|line
                                if (isExternal(map[y*width+x-1])) {
                                    drawTile(screen, x, y-1, TileType::WALL_TOP_RIGHT_TOP);
                                    drawTile(screen, x,   y, TileType::WALL_TOP_RIGHT_BOTTOM);
                                } else {
                                    drawTile(screen, x, y-1, TileType::WALL_BACK_LEFT_TOP);
                                    drawTile(screen, x,   y, TileType::WALL_BACK_LEFT_BOTTOM);
                                }
                            } else {
                                std::cout << "Incorrect level map. # right of # without other walls nearby." << std::endl;
                            }
                        } else {//no #s above or left
                            if (map[y*width+x] == '#' && map[(y-1)*width+x+1] == '#') { // Г-angle
                                if (isExternal(map[y*width+x+1])) {
                                    drawTile(screen, x, y-1, TileType::WALL_TOP_LEFT_TOP_AMBIENT);
                                    drawTile(screen, x,   y, TileType::WALL_TOP_LEFT_BOTTOM);
                                } else {
                                    drawTile(screen, x, y-1, TileType::WALL_BACK_RIGHT_TOP);
                                    drawTile(screen, x,   y, TileType::WALL_BACK_RIGHT_BOTTOM);
                                }
                            } else {
                                std::cout << "Incorrect level map. # without two neighbours" << std::endl;
                            }
                        }
                    }
                }
            }
            if (map[(y-1)*width+x] == 'x') {
                if (y == 1) {
                    if (map[(y-1)*width+x+1] == 'x') {
                        drawTile(screen, x, y-1, TileType::PASSAGE_TOP_LEFT_TOP);
                        drawTile(screen, x,   y, TileType::PASSAGE_TOP_LEFT_BOTTOM);
                    } else if (map[(y-1)*width+x-1] == 'x') {
                        drawTile(screen, x, y-1, TileType::PASSAGE_TOP_RIGHT_TOP);
                        drawTile(screen, x,   y, TileType::PASSAGE_TOP_RIGHT_BOTTOM);
                    } else {
                        std::cout << "Incorrect level map. 'x' cannot be alone" << std::endl;
                    }
                } else if (x == 0) {
                    if (map[y*width+x] == 'x') {
                        drawTile(screen, x, y-1, TileType::WALL_FRONT_LEFT_TOP);
                        drawTile(screen, x,   y, TileType::WALL_FRONT_LEFT_BOTTOM);
                    } else if (map[(y-2)*width+x] == 'x') {
                        drawTile(screen, x,   y, TileType::WALL_BACK_LEFT_TOP);
                    } else {
                        std::cout << "Incorrect level map. 'x' cannot be alone" << std::endl;
                    }
                } else if (x == width - 1) {
                    if (map[y*width+x] == 'x') {
                        drawTile(screen, x, y-1, TileType::WALL_FRONT_RIGHT_TOP);
                        drawTile(screen, x,   y, TileType::WALL_FRONT_RIGHT_BOTTOM);
                    } else if (map[(y-2)*width+x] == 'x') {
                        drawTile(screen, x,   y, TileType::WALL_BACK_RIGHT_TOP);
                    } else {
                        std::cout << "Incorrect level map. 'x' cannot be alone" << std::endl;
                    }
                } else {
                    std::cout << "Incorrect level map. 'x' can be only on top, left and right borders" << std::endl;
                }
            }
        }
    }
}

void LevelMap::drawTile(Image &screen, int x, int y, TileType type) {
    static Image tileset("../resources/Dungeon_Tileset_inverted.png");

    for (int j = 0; j < 2*tileSize; j++) {
        for (int i = 0; i < 2*tileSize; i++) {
            screen.PutPixel(i+ 2*x*tileSize, j + 2*y*tileSize, 
                            tileset.GetPixel(tile_map[type].x*tileSize + i/2, tile_map[type].y*tileSize + j/2));
        }
    }

}