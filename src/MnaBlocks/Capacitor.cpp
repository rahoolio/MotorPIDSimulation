#include "MnaBlocks/Capacitor.h"

using namespace ELCT350;
using namespace ELCT350::Mna;

#pragma region Constructors
Capacitor::Capacitor()
  : SimulationBlock(1),
    MnaBlock(2),
    _voltage(0.0)
{
}
#pragma endregion

#pragma region MnaBlock
void Capacitor::step(double dTimeStep, double dCurrentTime)
{
  double jacobian = _parameters[Capacitance] / dTimeStep;
  //Jacobian stamp
  setJacobian(Positive, Positive, jacobian);
  setJacobian(Positive, Negative, -jacobian);
  setJacobian(Negative, Positive, -jacobian);
  setJacobian(Negative, Negative, jacobian);

  double intercept = jacobian * _voltage;
  setIntercept(Positive, intercept);
  setIntercept(Negative, -intercept);
}

void Capacitor::postStep()
{
  _voltage = getAcross(Positive) - getAcross(Negative);
}
#pragma endregion