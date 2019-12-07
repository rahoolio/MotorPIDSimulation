#include "MnaBlocks/Diode.h"
#include <cmath>

using namespace ELCT350;
using namespace ELCT350::Mna;

const double CONVERGENCE_TYPE_VOLTAGE_THRESHOLD = 0.75;

#pragma region Constructors
Diode::Diode()
  : SimulationBlock(3),
    MnaBlock(2)
{
}
#pragma endregion

#pragma region MnaBlock
void Diode::step(double dTimeStep, double dCurrentTime)
{
  double voltage = getAcross(Anode) - getAcross(Cathode);
  double current = getThrough(Anode);

  if ((voltage > CONVERGENCE_TYPE_VOLTAGE_THRESHOLD) && (current <= _parameters[SaturationCurrent]))
  {
    voltage = CONVERGENCE_TYPE_VOLTAGE_THRESHOLD;
  }

  //if ((voltage <= CONVERGENCE_TYPE_VOLTAGE_THRESHOLD) || (current <= _parameters[SaturationCurrent]))
    current =
      _parameters[SaturationCurrent] * exp(voltage / _parameters[ThermalVoltage]) - _parameters[SaturationCurrent];
  //else
    //voltage = _parameters[ThermalVoltage] * log(current / _parameters[SaturationCurrent] + 1.0);

  double currentPlusSaturation = current + _parameters[SaturationCurrent];
  double jacobian = currentPlusSaturation / _parameters[ThermalVoltage];
  if(jacobian < 1e-12)
    jacobian = 1e-12;
  setJacobian(Anode, Anode, jacobian);
  setJacobian(Anode, Cathode, -jacobian);
  setJacobian(Cathode, Anode, -jacobian);
  setJacobian(Cathode, Cathode, jacobian);

  double intercept = voltage * jacobian - current;
  setIntercept(Cathode, -intercept);
  setIntercept(Anode, intercept);
}
#pragma endregion
