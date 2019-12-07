#include "SignalBlock.h"
#include <exception>

using namespace ELCT350;
using namespace std;

SignalBlock::SignalBlock(size_t numInputs, size_t numOutputs)
{

  for (size_t index = 0; index < numInputs; ++index)
  {
    _inputs.push_back(0.0);
  }

  for (size_t index = 0; index < numOutputs; ++index)
  {
    _outputs.push_back(0.0);
  }
}

double SignalBlock::getOutPortValue(size_t index) const
{
  if (index >= _outputs.size())
    throw exception("Out of range");

  return _outputs[index];
}

void SignalBlock::setInputPortValue(size_t index, double value)
{
  if (index >= _inputs.size())
    throw exception("Out of range");

  _inputs[index] = value;
}