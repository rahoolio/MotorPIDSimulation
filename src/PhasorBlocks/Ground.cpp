#include "PhasorBlocks/Ground.h"

using namespace ELCT350;
using namespace ELCT350::Phasor;

Ground::Ground()
      : SimulationBlock(0),
        PhasorBlock(1)
{
  getPort(Reference).Reference = true;
}

void Ground::step(double dTimeStep, double frequency)
{
}