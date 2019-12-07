#include "SimulationBlock.h"
#include "SimulatorException.h"

using namespace ELCT350;
using namespace std;

SimulationBlock::SimulationBlock(size_t numParameters)
{
  for (size_t index = 0; index < numParameters; ++index)
  {
    _parameters.push_back(0.0);
  }
}

void SimulationBlock::setParameter(size_t index, double value)
{
  if (index >= _parameters.size())
    throw SimulatorException("Out of range");

  _parameters[index] = value;
}
