#pragma once
#include "Zlib/zlib.h"
#include <vector>

class ZLibBase
{
public:
  ZLibBase();
  ~ZLibBase();

  void setInput(const char* input, size_t size);

  const char* getOutput() const;
  unsigned long getOutputSize() const;

protected:
  const char* m_input;
  size_t m_inputSize;

  std::vector<char> m_output;

  // Type of m_outputSize must be match with type m_zlibStream.total_out,
  // otherwise may overflow long and value of m_outputSize will be too big.
  unsigned long m_outputSize;
};

