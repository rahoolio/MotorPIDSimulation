#include "SignalBlocks/PIDController.h"

using namespace ELCT350::Signal;

PIDController::PIDController()
			 : SimulationBlock(3), SignalBlock(1, 1),
			   _integral(0.0), _pre_error(0.0)
{
}

void PIDController::step(double timeStep, double currentTime)
{
	// Proportional term
	double Pout = _parameters[Kp] * _inputs[Error];

	// Integral term
	_integral += _inputs[Error] * timeStep;
	double Iout = _parameters[Ki] * _integral;

	// Derivative term
	double derivative = (_inputs[Error] - _pre_error) / timeStep;
	double Dout = _parameters[Kd] * derivative;

	// Calculate total output
	double output = Pout + Iout + Dout;

	// Save error to previous error
	_pre_error = _inputs[Error];

	_outputs[CorrectedOutput] = output;
}