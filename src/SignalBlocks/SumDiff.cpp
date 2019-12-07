#include "SignalBlocks/SumDiff.h"

using namespace ELCT350::Signal;

SumDiff::SumDiff()
	   : SimulationBlock(0), SignalBlock(2, 1)
{
}

void SumDiff::step(double timeStep, double currentTime)
{
	_outputs[Difference] = _inputs[Reference] - _inputs[Signal];
}