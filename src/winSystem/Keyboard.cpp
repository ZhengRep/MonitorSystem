#include "Keyboard.h"

void Keyboard::getState(BYTE state[256])
{
  if (!GetKeyboardState(state)) {
    throw SystemException();
  }
}

void Keyboard::setState(BYTE state[256])
{
  if (!SetKeyboardState(state)) {
    throw SystemException();
  }
}

bool Keyboard::isKeyPressed(BYTE vkCode)
{
  return (GetAsyncKeyState(vkCode) & 0x8000) != 0;
}

