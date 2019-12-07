#include "PhasorBlocks/Resistor.h"

using namespace ELCT350;
using namespace ELCT350::Phasor;

#pragma region Constructors
Resistor::Resistor()
        : SimulationBlock(1),
          PhasorBlock(2)
{
}
#pragma endregion

#pragma region MnaBlock
void Resistor::step(double dTimeStep, double frequency)
{
  double conductance = 1.0 / _parameters[Resistance];
  //Jacobian stamp
  setJacobian(Positive, Positive, conductance);
  setJacobian(Positive, Negative, -conductance);
  setJacobian(Negative, Positive, -conductance);
  setJacobian(Negative, Negative, conductance);
}
#pragma endregion