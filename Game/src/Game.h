#pragma once

#include <GameEngine.h>
#include <SDL_rect.h>
#include "TestBox.h"

union SDL_Event;
struct SDL_Texture;

class Game: public GameEngine
{
  friend class GameEngine;

public:
  ~Game();

protected:
  Game();

  void InitializeImpl();
  void UpdateImpl(float dt);
  void DrawImpl(SDL_Renderer *renderer, float dt);

  void Reset();
  void CalculateDrawOrder(std::vector<GameObject *>& drawOrder);

  TestBox _firstBox;
  TestBox _secondBox;
  TestBox _thirdBox;
};