#include "ZLibBase.h"

ZLibBase::ZLibBase()
  : m_input(0), m_inputSize(0), m_output(0), m_outputSize(0)
{
}

ZLibBase::~ZLibBase()
{
}

void ZLibBase::setInput(const char* input, size_t size)
{
  m_input = input;
  m_inputSize = size;
}

const char* ZLibBase::getOutput() const
{
  return &m_output.front();
}

unsigned long ZLibBase::getOutputSize() const
{
  return m_outputSize;
}
