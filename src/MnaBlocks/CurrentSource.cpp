#include "MnaBlocks/CurrentSource.h"

using namespace ELCT350;
using namespace ELCT350::Mna;

#pragma region Constructors
CurrentSource::CurrentSource()
        : SimulationBlock(1),
          MnaBlock(2)
{
}
#pragma endregion

#pragma region MnaBlock
void CurrentSource::step(double dTimeStep, double dCurrentTime)
{
  //Jacobian stamp
  setJacobian(Positive, Positive, 0);
  setJacobian(Positive, Negative, 0);
  setJacobian(Negative, Positive, 0);
  setJacobian(Negative, Negative, 0);

  setIntercept(Positive, _parameters[Current]);
  setIntercept(Negative, -_parameters[Current]);
}
#pragma endregion