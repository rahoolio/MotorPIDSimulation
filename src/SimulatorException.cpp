#include "SimulatorException.h"

using namespace ELCT350;
using namespace std;

SimulatorException::SimulatorException(const string& message) noexcept
                  : _message(message)
{
}

const char* SimulatorException::what() const noexcept
{
  return _message.c_str();
}
