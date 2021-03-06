#ifndef MAIN_VIRTUAL_CAM_H
#define MAIN_VIRTUAL_CAM_H

#include "LevelMap.h"
#include "Image.h"
#include "Player.h"
#include "EndScreen.h"
#include <string>
#include <vector>

struct VirtualCamera 
{
    explicit VirtualCamera(const std::string &path, int width, int height, int channels);

    void update();
    void ProcessInput(MovementDir dir, double angle_coef);
    void nextLevel();
    void setLevel(int n);
    void hide(Image &screen);
    Image& screen() { return screenBuffer; }

private:
    std::vector<LevelMap>::iterator level;
    Image levelBuffer;
    Image screenBuffer;
    Player player;
    EndScreen endScreen;
    std::vector<LevelMap> levelList;
    bool active = 1;
    int levelChanged = 0;
    int channels = 4;
    double transitionStart = -1;
};

#endif //MAIN_VIRTUAL_CAM_H