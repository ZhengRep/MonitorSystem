#include "EOFException.h"

EOFException::EOFException()
  :IOException(_T("End of file reached"))
{
}

EOFException::~EOFException()
{
}
