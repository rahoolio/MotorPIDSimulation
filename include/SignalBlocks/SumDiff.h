#pragma once
#include "SignalBlock.h"

namespace ELCT350
{
  namespace Signal
  {
    class SumDiff final : public SignalBlock
    {
    public:
      #pragma region Enumerations
	  enum InputSignalPorts : size_t
	  {
		Reference,
		Signal
	  };

	  enum OutputSignalPorts : size_t
	  {
		Difference
	  };
      #pragma endregion

      #pragma region Constructors and Destructors
	  SumDiff();
      #pragma endregion

	  #pragma region SignalBlock
	  virtual void step(double timeStep, double currentTime) override;
	  #pragma endregion
    };
  }
}