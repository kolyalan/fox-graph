#ifndef MAIN_END_SCREEN_H
#define MAIN_END_SCREEN_H

#include "Image.h"

extern "C" {
    double glfwGetTime();
}

struct EndScreen {
    EndScreen() : GameOver("../resources/game_over.png"), Win("../resources/you_win.png") {}
    
    void ShowWin(Image &screen);
    void ShowFail(Image &screen);
    bool timer_active() {
        return time_started != -1 && (glfwGetTime() - time_started) < 4;
    }
    void reset() { time_started = -1;}

private:
    Image GameOver;
    Image Win;
    void show(Image &screen, Image &pic);
    double time_started = -1;
};

#endif