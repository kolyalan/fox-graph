#include "Animation.h"
#include "math.h"
#include <iostream>

void Animation::draw(Image &screen, int x, int y) {
    if (isRunning()) {
        update();
    }
    screen.PutTile(tileset, curFrame*tileSize, type*tileSize, x, y, 2, lookLeft);
}

void Animation::update() {
    double curTime = glfwGetTime();
    double deltaTime = curTime - lastFrameTime;

    //std::cout << "curTime " << curTime << ", lastFrameTime " << lastFrameTime << std::endl;
    double frameDelta = duration[type] / frameNum[type];
    if (deltaTime < frameDelta) {
        return;
    }
    curFrame += floor(deltaTime / frameDelta);
    lastFrameTime = curTime;
    if (repeat[type]) {
        curFrame %= frameNum[type];
    } else {
        if (curFrame >= frameNum[type]) {
            curFrame = frameNum[type] - 1;
            stop();
        }
    }
}