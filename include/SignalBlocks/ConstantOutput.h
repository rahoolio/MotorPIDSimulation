#pragma once
#include "SignalBlock.h"

namespace ELCT350
{
  namespace Signal
  {
    class ConstantOutput final : public SignalBlock
    {
    public:
      #pragma region Enumerations
	  enum OutputSignalPorts : size_t
	  {
		Constant
	  };
      #pragma endregion

      #pragma region Constructors and Destructors
	  ConstantOutput(double);
      #pragma endregion

	  #pragma region SignalBlock
	  virtual void step(double timeStep, double currentTime) override;
	  #pragma endregion
    };
  }
}