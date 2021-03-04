#ifndef MAIN_ANIMATION_H
#define MAIN_ANIMATION_H

#include "Image.h"

extern "C" {
    double glfwGetTime();
}

enum AnimType {
    IDLE = 0,
    IDLE_LOOKING,
    WALKING,
    JUMPING,
    DAMAGE,
    SLEEP,
    DIE,
    N_ANIM_TYPES
};

static int frameNum[]    = {5,     14,    8, 11, 5, 6,   7};
static bool repeat[]     = {1,      1,    1,  0, 1, 0,   0};
static double duration[] = {0.6, 1.68,    1,  1, 1, 1, 0.8};

struct Animation {
    Animation(const std::string &path) : 
            tileset(path),  type(IDLE), curFrame(0), lastFrameTime(-1) {}
    
    void draw(Image &screen, int x, int y);
    bool isRunning() { return lastFrameTime != -1;}
    void setDir(bool left) {lookLeft = left;}
    void setType(AnimType AnimationType) {
        if (type <= AnimType::N_ANIM_TYPES) {
            if (type == AnimType::JUMPING) {
                curFrame = 0;
            }
            type = AnimationType;
            if (curFrame >= frameNum[type] || type == AnimType::DIE || type == AnimType::DAMAGE) {
                curFrame = 0;
            }
            if (type == AnimType::JUMPING) {
                curFrame = 2;
            }
        }
    }
    void start() {lastFrameTime = glfwGetTime();}
    void stop() {lastFrameTime = -1;}

private:
    Image tileset;
    AnimType type;
    int curFrame;
    double lastFrameTime = -1;
    bool lookLeft = 0;
    void update();
};

#endif //MAIN_ANIMATION_H