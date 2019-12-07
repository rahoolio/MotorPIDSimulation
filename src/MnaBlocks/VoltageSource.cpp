#include "MnaBlocks/VoltageSourceDC.h"

using namespace ELCT350;
using namespace ELCT350::Mna;

#pragma region Constructors
VoltageSourceDC::VoltageSourceDC()
               : SimulationBlock(1),
                 MnaBlock(3)
{
}
#pragma endregion

#pragma region MnaBlock
void VoltageSourceDC::step(double dTimeStep, double dCurrentTime)
{
  //Jacobian stamp
  setJacobian(Positive, SupplyCurrent_Internal, -1);
  setJacobian(Negative, SupplyCurrent_Internal, 1);
  setJacobian(SupplyCurrent_Internal, Positive, 1);
  setJacobian(SupplyCurrent_Internal, Negative, -1);

  //Intercept stamp
  setIntercept(SupplyCurrent_Internal, _parameters[Voltage]);
}
#pragma endregion