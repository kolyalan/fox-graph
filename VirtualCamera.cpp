#include "VirtualCamera.h"
#include <iostream>
#include <filesystem>
#include <math.h>

VirtualCamera::VirtualCamera(const std::string &path, int screen_width, int screen_height, int channels) 
    : screenBuffer(screen_width, screen_height, channels),
    endScreen(),
    channels(channels)
{   
    std::filesystem::directory_iterator dir(path);
    for (auto &p : dir) {
        if (p.exists()) {
            //std::cout << p.path() << std::endl;
            levelList.emplace_back(p.path());
        }
    }
    if (levelList.size() >= 1) {
        level = levelList.begin();
    }
    levelBuffer = Image(level->Width()*tileSize*2, (level->Height()+1)*tileSize*2, channels),
    player.SetPos(level->getStartPos()),
    level->draw(levelBuffer);
}

void VirtualCamera::update() {
    bool win = level->PlayerMustWin(player.Pos().x, player.Pos().y);
    bool die = level->PlayerMustDie(player.Pos().x, player.Pos().y);
    if (active || endScreen.timer_active() || levelChanged) {
        levelBuffer.GetSubImage(screenBuffer, player.Pos().x - screenBuffer.Width()/2,
                                              player.Pos().y - screenBuffer.Height()/2);
    }
    if (levelChanged) {
        double cur_time = glfwGetTime();
        if (transitionStart < 0) {
            transitionStart = cur_time;
        }
        hide(screenBuffer);
        if (cur_time - transitionStart > 2) {
            if (levelChanged == 1) {
                nextLevel();
                levelChanged = 2;
            }
        }
        if (cur_time - transitionStart > 4) {
            transitionStart = -1;
            levelChanged = 0;
            active = 1;
        }
        player.Draw(screenBuffer);
        return;
    }
    if ((!active && die) || (!player.isJumping() && die)) {
        endScreen.ShowFail(screenBuffer);
        if (active) {
            player.Die();
            active = 0;
        }
    }
    if (win && !die) {
        if (level + 1 == levelList.end()) {
            endScreen.ShowWin(screenBuffer);
            if (!endScreen.timer_active()) {
                player.Win();
            }
            if (active) {
                active = 0;
            }
        } else if (!levelChanged && active) {
            levelChanged = 1;
            active = 0;
        }
    }
    player.Draw(screenBuffer);
}

void VirtualCamera::ProcessInput(MovementDir dir, double angle_coef) {
    if (active) {
        player.ProcessInput(dir, angle_coef, *level);
    } else {
        if (!levelChanged && dir == MovementDir::JUMP) {
            setLevel(0);
            player.Arise();
            endScreen.reset();
            levelChanged = 2;
            transitionStart = glfwGetTime() - 2;
        }
    }
}

void VirtualCamera::nextLevel() {
    level++;
    levelBuffer = Image(level->Width()*tileSize*2, (level->Height()+1)*tileSize*2, channels),
    player.SetPos(level->getStartPos()),
    level->draw(levelBuffer);
}

void VirtualCamera::setLevel(int n) {
    if (n < 0 || n >= levelList.size()) {
        std::cout << "Incorrect level requested: " << n << std::endl;
        return;
    }
    level = levelList.begin() + n;
    levelBuffer = Image(level->Width()*tileSize*2, (level->Height()+1)*tileSize*2, channels),
    player.SetPos(level->getStartPos()),
    level->draw(levelBuffer);
}

void VirtualCamera::hide(Image &screen) {
    double c = fabs((glfwGetTime() - transitionStart - 2) / 2);
    if (c > 1) return;
    for (int j = 0; j < screen.Height(); j++) {
        for (int i = 0; i < screen.Width(); i++) {
            Pixel color = screen.GetPixel(i, j);
            color.r *= c;
            color.g *= c;
            color.b *= c;
            screen.PutPixel(i, j, color);
        }
    }
}