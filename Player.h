#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "LevelMap.h"
#include "Animation.h"

#define FOX_PATH "../resources/FoxSpriteSheetInverted.png"

extern "C" {
    double glfwGetTime();
}

struct Point
{
  int x;
  int y;
  Point(int x, int y) : x(x), y(y) {}
  Point(MapPoint pos) : x(pos.x * 2 *tileSize + tileSize), y(pos.y*2*tileSize + tileSize) {}
};

enum class MovementDir
{
  NO = 0,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  JUMP
};

struct Player
{
  explicit Player(Point pos = {10, 10}) :
                 coords(pos), old_coords(coords), fox(FOX_PATH) {};
  explicit Player(MapPoint pos) :
                 coords(pos), old_coords(coords), fox(FOX_PATH) {};

  bool Moved() const;
  void ProcessInput(MovementDir dir, double angle_coef, LevelMap &level);
  void Draw(Image &screen);
  void Die() { dead = 1; fox.setType(AnimType::DIE);}
  void Win() { fox.stop();}
  bool isJumping() const {return inAir;}
  Point Pos() const { return coords; }
  void SetPos(MapPoint pos) {coords = pos;}

private:
  Point coords {10, 10};
  Point old_coords {10, 10};
  Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
  int move_speed = 4;
  Animation fox;
  double lastMoveTime = 0;
  bool dead = 0;
  bool jumps = 0;
  bool inAir = 0;
  double jumpStarted = -1;

};

#endif //MAIN_PLAYER_H
