#pragma once
#include "SignalBlock.h"

namespace ELCT350
{
  namespace Signal
  {
    class PIDController final : public SignalBlock
    {
    public:
      #pragma region Enumerations
	  enum Parameters : size_t
	  {
		Kp,
		Ki,
		Kd
	  };

	  enum InputSignalPorts : size_t
	  {
		Error
	  };

	  enum OutputSignalPorts : size_t
	  {
		CorrectedOutput
	  };
      #pragma endregion

      #pragma region Constructors and Destructors
	  PIDController();
      #pragma endregion

	  #pragma region SignalBlock
	  virtual void step(double timeStep, double currentTime) override;
	  #pragma endregion
	private:
	  #pragma region variables
	  double _integral;
	  double _pre_error;
	  #pragma endregion
    };
  }
}