#include "PhasorBlocks/Capacitor.h"

using namespace ELCT350;
using namespace ELCT350::Phasor;

#pragma region Constructors
Capacitor::Capacitor()
  : SimulationBlock(1),
    PhasorBlock(2)
{
}
#pragma endregion

#pragma region MnaBlock
void Capacitor::step(double dTimeStep, double frequency)
{
  auto jacobian = std::complex<double>(0.0, _parameters[Capacitance] * 2.0 * 3.14 * frequency);
  //Jacobian stamp
  setJacobian(Positive, Positive, jacobian);
  setJacobian(Positive, Negative, -jacobian);
  setJacobian(Negative, Positive, -jacobian);
  setJacobian(Negative, Negative, jacobian);
}
#pragma endregion