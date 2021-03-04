#include "Player.h"
#include "math.h"
#include <iostream>

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir, double angle_coef, LevelMap &level)
{
  int move_dist = ceil(move_speed * angle_coef);
  Point tmp_coords = coords;
  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y;
      coords.y -= move_dist;
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      coords.y += move_dist;
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
      coords.x -= move_dist;
      fox.setDir(1);
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
      coords.x += move_dist;
      fox.setDir(0);
      break;
    case MovementDir::JUMP:
      if (!inAir) {
        jumps = 1;
        jumpStarted = glfwGetTime();
        fox.setType(AnimType::JUMPING);
      }
      break;
    default:
      break;
  }
  if (!level.PlayerCanMove(coords.x, coords.y)) {
    coords = tmp_coords;
  }
}

void Player::Draw(Image &screen)
{
  //static Image sprite("../resources/FoxSpriteSheetInverted.png");
  if (!fox.isRunning()) {
    fox.start();
  }
  if (jumps) {
    double time = glfwGetTime();
    if (time - jumpStarted > 1) {
      jumps = 0;
      jumpStarted = -1;
    } else if (time - jumpStarted > 0.45) {
      inAir = 0;
    } else {
      inAir = 1;
    }
  }
  if (!dead) {
    if(Moved())
    {
      if (!jumps) {
        fox.setType(AnimType::WALKING);
      }
      old_coords = coords;
      lastMoveTime = glfwGetTime();
    } else {
      if (!jumps) {
        if (glfwGetTime() - lastMoveTime > 5) {
          fox.setType(AnimType::IDLE_LOOKING);
        } else {
          fox.setType(AnimType::IDLE);
        }
      }
    }
  }
  Point middle = {screen.Width()/2-tileSize, screen.Height()/2-tileSize};
  fox.draw(screen, middle.x, middle.y);
}