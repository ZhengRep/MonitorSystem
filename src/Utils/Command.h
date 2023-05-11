#pragma once
class Command
{
public:
  Command(){};
  virtual ~Command(){};

  virtual void execute() = 0;
};

