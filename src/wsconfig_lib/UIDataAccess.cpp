#include "UIDataAccess.h"
#include "Utils/StringParser.h"

UIDataAccess::UIDataAccess()
{
}

bool UIDataAccess::queryValueAsInt(TextBox* textBox, int* value)
{
  _ASSERT(textBox != 0);

  StringStorage textBoxText;

  textBox->getText(&textBoxText);

  return StringParser::parseInt(textBoxText.getString(), value);
}

bool UIDataAccess::queryValueAsUInt(TextBox* textBox, unsigned int* value)
{
  _ASSERT(textBox != 0);

  StringStorage textBoxText;

  textBox->getText(&textBoxText);

  return StringParser::parseUInt(textBoxText.getString(), value);
}
