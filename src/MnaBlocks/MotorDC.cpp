#include "MnaBlocks/MotorDC.h"

using namespace ELCT350;
using namespace ELCT350::Mna;

#pragma region Constructors
MotorDC::MotorDC()
  : SimulationBlock(5),
    MnaBlock(3),
	_velocity(0.0),
	_currentNegative(0.0),
	_currentPositive(0.0)
{
}
#pragma endregion

#pragma region MnaBlock
void MotorDC::step(double dTimeStep, double dCurrentTime)
{
  double commonCoefficient1 = dTimeStep / (_parameters[L] + (dTimeStep * _parameters[R]));
  double commonCoefficientK = commonCoefficient1 * _parameters[K];
  double velocity = -(commonCoefficientK * _parameters[K]) - (_parameters[J] / dTimeStep) - _parameters[B];

  //Jacobian stamp
  setJacobian(Negative, Negative, commonCoefficient1);
  setJacobian(Negative, Positive, -commonCoefficient1);
  setJacobian(Negative, Velocity, -commonCoefficientK);

  setJacobian(Positive, Negative, -commonCoefficient1);
  setJacobian(Positive, Positive, commonCoefficient1);
  setJacobian(Positive, Velocity, commonCoefficientK);

  setJacobian(Velocity, Negative, -commonCoefficientK);
  setJacobian(Velocity, Positive, commonCoefficientK);
  setJacobian(Velocity, Velocity, velocity);

  double commonCoefficient2 = _parameters[L] / (_parameters[L] + (dTimeStep * _parameters[R]));
  setIntercept(Negative, -commonCoefficient2 * _currentPositive);
  setIntercept(Positive, commonCoefficient2 * _currentNegative);
  double interceptVelocity = ((-_parameters[J] / dTimeStep) * _velocity) - (commonCoefficient2 * _parameters[K] * _currentPositive);
  setIntercept(Velocity, interceptVelocity);
}

void MotorDC::postStep()
{
	_velocity = getAcross(Velocity);
	_currentNegative = getAcross(Negative);
	_currentPositive = getAcross(Positive);
}
#pragma endregion