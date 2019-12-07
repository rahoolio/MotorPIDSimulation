#include "SignalBlocks/ConstantOutput.h"

using namespace ELCT350::Signal;

ConstantOutput::ConstantOutput(double value)
              : SimulationBlock(0), SignalBlock(0, 1)
{
	_outputs[Constant] = value;
}

void ConstantOutput::step(double timeStep, double currentTime)
{
}