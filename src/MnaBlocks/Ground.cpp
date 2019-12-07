#include "MnaBlocks/Ground.h"

using namespace ELCT350;
using namespace ELCT350::Mna;

Ground::Ground()
      : SimulationBlock(0),
        MnaBlock(1)
{
  getPort(Reference).Reference = true;
}

void Ground::step(double dTimeStep, double dCurrentTime)
{
}