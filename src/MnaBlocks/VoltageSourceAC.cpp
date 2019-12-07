#include "MnaBlocks/VoltageSourceAC.h"
#include <cmath>

using namespace ELCT350;
using namespace ELCT350::Mna;

#pragma region Constructors
VoltageSourceAC::VoltageSourceAC()
               : SimulationBlock(4),
                 MnaBlock(2)
{
}
#pragma endregion

#pragma region MnaBlock
void VoltageSourceAC::step(double dTimeStep, double dCurrentTime)
{
  //Jacobian stamp
  /*
  setJacobian(Positive, SupplyCurrent_Internal, -1);
  setJacobian(Negative, SupplyCurrent_Internal, 1);
  setJacobian(SupplyCurrent_Internal, Positive, 1);
  setJacobian(SupplyCurrent_Internal, Negative, -1);

  //Intercept stamp
  setIntercept(SupplyCurrent_Internal,
               _parameters[Voltage] * sin(2*3.14*_parameters[Frequency]*dCurrentTime +
                                          _parameters[Phase] * 3.14/180.0) +
               _parameters[Offset]);
  */

  double voltage = _parameters[Voltage] * sin(2 * 3.14 * _parameters[Frequency] * dCurrentTime +
    _parameters[Phase] * 3.14 / 180.0) +
    _parameters[Offset];
  setJacobian(Positive, Positive, 1e3);
  setJacobian(Positive, Negative, -1e3);
  setJacobian(Negative, Positive, -1e3);
  setJacobian(Negative, Negative, 1e3);

  setIntercept(Positive, voltage * 1e3);
  setIntercept(Negative, -voltage * 1e3);
}
#pragma endregion
