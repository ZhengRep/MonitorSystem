#include "CommonInputValidation.h"
#include "Utils/StringParser.h"
#include "gui/BalloonTip.h"
#include "mnt_server/resource.h"
#include "UIDataAccess.h"

CommonInputValidation::CommonInputValidation()
{
}

bool CommonInputValidation::validatePort(TextBox* textBox)
{
  bool validationOk = true;

  int parsedPortValue = 0;

  if (!UIDataAccess::queryValueAsInt(textBox, &parsedPortValue)) {
    validationOk = false;
  } else {
    if (parsedPortValue <= 0 || parsedPortValue > 65535) {
      validationOk = false;
    }
  }

  if (!validationOk) {
    notifyValidationError(textBox, StringTable::getString(IDS_PORT_RANGE_ERROR));
  }

  return validationOk;
}

bool CommonInputValidation::validateUINT(TextBox* textBox, const TCHAR* messageText)
{
  bool validationOk = true;

  unsigned int parsedValue = 0;

  if (!UIDataAccess::queryValueAsUInt(textBox, &parsedValue)) {
    validationOk = false;
  }

  if (!validationOk) {
    notifyValidationError(textBox, messageText);
  }

  return validationOk;
}

void CommonInputValidation::notifyValidationError(TextBox* textBox, const TCHAR* messageText)
{
  BalloonTip invalidInputTip;

  invalidInputTip.setIconType(TTI_WARNING);
  invalidInputTip.setText(messageText);
  invalidInputTip.setTitle(StringTable::getString(IDS_CAPTION_BAD_INPUT));

  textBox->showBalloonTip(&invalidInputTip);
  textBox->setFocus();
}
