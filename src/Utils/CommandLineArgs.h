#pragma once
#include "Utils/CommonHeader.h"
#include <vector>

class CommandLineArgs
{
public:
  CommandLineArgs();
  virtual ~CommandLineArgs();
  void getArgVector(std::vector<StringStorage>* out) const;

protected:
  std::vector<StringStorage> m_args;
};

