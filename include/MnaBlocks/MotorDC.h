#pragma once
#include "MnaBlock.h"

namespace ELCT350
{
  namespace Mna
  {
    class MotorDC final : public MnaBlock
    {
    public:
      #pragma region Enumerations
      enum Ports : size_t
      {
        Negative,
        Positive,
		Velocity
      };

      enum Parameters : size_t
      {
	    L,
	    R,
	    B,
	    K,
	    J
      };
      #pragma endregion

      #pragma region Constructors and Destructors
	  MotorDC();
      #pragma endregion

      #pragma region MnaBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      virtual void postStep() override;
      #pragma endregion
    private:
      #pragma region variables
	  double _velocity;
	  double _currentNegative;
      double _currentPositive;
      #pragma endregion
    };
  }
}