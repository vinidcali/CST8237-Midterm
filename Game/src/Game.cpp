#include "Game.h"
#include <GameObject.h>
#include <SDL.h>
#include <math.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <InputManager.h>

#define PIXELS_PER_METRE 8.0f

// Initializing our static member pointer.
GameEngine* GameEngine::_instance = nullptr;

GameEngine* GameEngine::CreateInstance()
{
  if (_instance == nullptr)
  {
    _instance = new Game();
  }
  return _instance;
}

Game::Game() : GameEngine()
{

}

Game::~Game()
{
}

void Game::InitializeImpl()
{
  SDL_SetWindowTitle(_window, "Game");

  _objects.push_back(&_firstBox);
  _objects.push_back(&_secondBox);
  _objects.push_back(&_thirdBox);

  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
    (*itr)->Initialize(_renderer);
  }

  // Get the window size so we can draw it in the center of the screen later.
  SDL_Point windowSize;
  SDL_GetWindowSize(_window, &windowSize.x, &windowSize.y);

  _firstBox.GetTransform().position = { windowSize.x / 2, windowSize.y / 2, 0 };
  _secondBox.GetTransform().position = { windowSize.x / 3, windowSize.y / 1, 0 };
  _thirdBox.GetTransform().position = { windowSize.x / 1, windowSize.y / 3, 0 };
}

void Game::UpdateImpl(float dt)
{
  InputManager::GetInstance()->Update(dt);

  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
    (*itr)->Update(dt);
  }
}

void Game::DrawImpl(SDL_Renderer *renderer, float dt)
{
  std::vector<GameObject *> renderOrder;
  CalculateDrawOrder(renderOrder);

  for (auto itr = renderOrder.begin(); itr != renderOrder.end(); itr++)
  {
    (*itr)->Draw(renderer, dt);
  }
}

void Game::CalculateDrawOrder(std::vector<GameObject *>& drawOrder)
{
  // SUPER HACK GARBAGE ALGO.
  drawOrder.clear();

  auto objectsCopy = _objects;
  auto farthestEntry = objectsCopy.begin();
  while (objectsCopy.size() > 0)
  {
    bool entryFound = true;
    for (auto itr = objectsCopy.begin(); itr != objectsCopy.end(); itr++)
    {
      if (farthestEntry != itr)
      {
        if ((*itr)->GetTransform().position.y < (*farthestEntry)->GetTransform().position.y)
        {
          entryFound = false;
          farthestEntry = itr;
          break;
        }
      }
    }

    if (entryFound)
    {
      GameObject *farthest = *farthestEntry;

      drawOrder.push_back(farthest);
      objectsCopy.erase(farthestEntry);
      farthestEntry = objectsCopy.begin();
    }
  }
}