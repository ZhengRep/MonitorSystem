#pragma once
#include "utils/CommonHeader.h"

class ClipboardListener
{
public:
  ClipboardListener();
  ~ClipboardListener();
  //Interface function
  virtual void onClipboardUpdate(const StringStorage* newClipboard) = 0;
};

