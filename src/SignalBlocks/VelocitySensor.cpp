#include "SignalBlocks/VelocitySensor.h"

using namespace ELCT350::Signal;

VelocitySensor::VelocitySensor()
              : SimulationBlock(0), MnaBlock(1), SignalBlock(0, 1)
{
}

void VelocitySensor::step(double timeStep, double dCurrentTime)
{
  _outputs[Velocity] = getAcross(Sensor);
}