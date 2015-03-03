#include "TestBox.h"
#include <SDL.h>
#include <SDL_image.h>

float _rotationSpeed = 1.0f;

TestBox::~TestBox()
{
  SDL_DestroyTexture(_testTexture);
}

void TestBox::Initialize(SDL_Renderer *renderer)
{
  // Load our texture.
  _testTexture = IMG_LoadTexture(renderer, "res/UV.jpg");
  _transform.rotation.z = 0.0f;

  /* Query the texture to get it's size. We then use it's size to offset where the texture will draw relative to,
  * since the default is it's top corner (0,0). */
  _testTextureBounds = { 0, 0, 0, 0 };
  SDL_QueryTexture(_testTexture, nullptr, nullptr, &_testTextureBounds.w, &_testTextureBounds.h);
  _testTextureBounds.x -= _testTextureBounds.w / 2;
  _testTextureBounds.y -= _testTextureBounds.h / 2;

  _testTextureBounds.w /= 4;
  _testTextureBounds.h /= 4;

  /* This center will only be the point we rotate around; not the point we draw relative to. So if I set this center to be the middle
  * of the texture, it will still draw at (0,0), and rotate around (250,250), which is where the middle of the texture would be. */
  _testTextureRotationCenter = { _testTextureBounds.w / 2, _testTextureBounds.h / 2 };

  _transform.scale.x = 1.0f;
  _transform.scale.y = 1.0f;
  _transform.scale.z = 1.0f;
}

void TestBox::Update(float dt)
{
  _transform.rotation.z += _rotationSpeed * dt;
}

void TestBox::Draw(SDL_Renderer *renderer, float dt)
{
  SDL_Rect drawRect = _testTextureBounds;
  drawRect.x += _transform.position.x;
  drawRect.y += _transform.position.y;

  drawRect.w *= _transform.scale.x;
  drawRect.h *= _transform.scale.y;

  SDL_RenderCopyEx(renderer, _testTexture, nullptr, &drawRect, MathUtils::ToRadians(_transform.rotation.z), &_testTextureRotationCenter, SDL_FLIP_NONE);
}

void TestBox::SetRotationSpeed(float rotationSpeed)
{
  _rotationSpeed = rotationSpeed;
}