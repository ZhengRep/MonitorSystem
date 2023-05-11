#include "OperationNotSupportedException.h"

OperationNotSupportedException::OperationNotSupportedException()
  :Exception(_T("Operation not supported"))
{
}

OperationNotSupportedException::~OperationNotSupportedException()
{
}
