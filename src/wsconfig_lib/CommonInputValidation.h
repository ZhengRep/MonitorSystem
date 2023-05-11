#pragma once
#include "gui/TextBox.h"

class CommonInputValidation
{
public:
  static bool validatePort(TextBox* textBox);
  static bool validateUINT(TextBox* textBox, const TCHAR* messageText);
  static void notifyValidationError(TextBox* textBox, const TCHAR* messageText);

private:
  CommonInputValidation();
};

