#include "EndScreen.h"

void EndScreen::ShowWin(Image &screen) {
    if (time_started == -1) {
        time_started = glfwGetTime();
    }
    show(screen, Win);
}

void EndScreen::ShowFail(Image &screen) {
    if (time_started == -1) {
        time_started = glfwGetTime();
    }
    show(screen, GameOver);
}

void EndScreen::show(Image &screen, Image &pic) {
    int x = (screen.Width() - pic.Width()) / 2;
    int y = (screen.Height() - pic.Height()) / 2;
    double c = 1 - (glfwGetTime() - time_started) / 4;
    if (c < 0) {
        c = 0;
        return;
    }
    for (int j = 0; j < screen.Height(); j++) {
        for (int i = 0; i < screen.Width(); i++) {
            Pixel color = screen.GetPixel(i, j);
            color.r *= c;
            color.g *= c;
            color.b *= c;
            screen.PutPixel(i, j, color);
            if (j >= y && j < y + pic.Height() && i >= x && i < x + pic.Width()) {
                screen.PutPixel(i, j, pic.GetPixel(i-x, j-y));
            }
        }
    }
}