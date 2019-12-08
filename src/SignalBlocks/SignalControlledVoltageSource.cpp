#include "SignalBlocks/SignalControlledVoltageSource.h"

using namespace ELCT350::Signal;

SignalControlledVoltageSource::SignalControlledVoltageSource()
              : SimulationBlock(2), MnaBlock(3), SignalBlock(1, 0)
{
}

void SignalControlledVoltageSource::step(double timeStep, double dCurrentTime)
{
  double voltage = _inputs[Signal] * _parameters[Gain];

  //Jacobian stamp
  // setJacobian(size_t row, size_t column, T value)
  setJacobian(Positive, SupplyCurrent_Internal, -1);
  // setJacobian(1, 2, -1)
  setJacobian(Negative, SupplyCurrent_Internal, 1);
  // setJacobian(0, 2, 1)
  setJacobian(SupplyCurrent_Internal, Positive, 1);
  // setJacobian(2, 1, 1)
  setJacobian(SupplyCurrent_Internal, Negative, -1);
  // setJacobian(2, 0, -1)

/*
	 0   1   2
0 {{ 0,  0,  1},
1  { 0,  0, -1},
2  {-1,  1,  0}}
*/

  //Intercept stamp
  // setIntercept(size_t row, T value)
  setIntercept(SupplyCurrent_Internal, voltage);
  // setIntercept(2, voltage);

/*
      0
0  {{ 0 },
1   { 0 },
2   { V }}
*/
}