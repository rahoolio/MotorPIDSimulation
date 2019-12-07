#include "PhasorBlocks/VoltageSourceAC.h"
#include <cmath>

using namespace ELCT350;
using namespace ELCT350::Phasor;
using namespace std;

#pragma region Constructors
VoltageSourceAC::VoltageSourceAC()
               : SimulationBlock(3),
                 PhasorBlock(2)
{
}
#pragma endregion

#pragma region MnaBlock
void VoltageSourceAC::step(double dTimeStep, double frequency)
{
  complex<double> voltage = polar(_parameters[Voltage], 2.0 * 3.14 * frequency);

  setJacobian(Positive, Positive, 1e3);
  setJacobian(Positive, Negative, -1e3);
  setJacobian(Negative, Positive, -1e3);
  setJacobian(Negative, Negative, 1e3);

  setIntercept(Positive, voltage * 1e3);
  setIntercept(Negative, -voltage * 1e3);
}
#pragma endregion
