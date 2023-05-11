#pragma once
#include "gui/TextBox.h"

class UIDataAccess
{
public:
  static bool queryValueAsInt(TextBox* textBox, int* value);
  static bool queryValueAsUInt(TextBox* textBox, unsigned int* value);

private:
  UIDataAccess();
};

