#pragma once
#include "MnaBlock.h"
#include "SignalBlock.h"

namespace ELCT350
{
  namespace Signal
  {
    class SignalControlledVoltageSource final : public MnaBlock, public SignalBlock
    {
    public:
      #pragma region Enumerations
	  enum MnaPorts : size_t
	  {
		Negative,
		Positive,
		SupplyCurrent_Internal
	  };

	  enum Parameters : size_t
	  {
		Gain
	  };

	  enum InputSignalPorts : size_t
	  {
		Signal
	  };
      #pragma endregion

      #pragma region Constructors and Destructors
	  SignalControlledVoltageSource();
      #pragma endregion

      #pragma region MnaBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      #pragma endregion
    };
  }
}