#include <iostream>
#include <fstream>
#include "Matrix.h"
#include "MnaSystem.h"
#include "MnaBlocks/Ground.h"
#include "MnaBlocks/Resistor.h"
#include "MnaBlocks/VoltageSourceAC.h"
#include "MnaBlocks/Capacitor.h"
#include "SignalBlocks/VelocitySensor.h"
#include "SignalBlocks/ConstantOutput.h"
#include "SignalBlocks/SumDiff.h"
#include "SignalBlocks/PIDController.h"
#include "SignalBlocks/SignalControlledVoltageSource.h"
#include "MnaBlocks/MotorDC.h"

#include "Plotter/Plotter.h"

using namespace ELCT350;
using namespace std;

const double TIME_STEP = 0.1;
const double FINAL_TIME = 40.0;

enum ErrorCodes
{
  OK
};

int main()
{
  MnaSystem system;
  Signal::VelocitySensor sensor;
  Signal::ConstantOutput co1 = Signal::ConstantOutput(1000.0);
  Signal::SumDiff sd1;
  Signal::PIDController pid1;
  Signal::SignalControlledVoltageSource scvs1;
  Mna::MotorDC mdc1;
  Mna::Ground g1;

  pid1.setParameter(Signal::PIDController::Kp, 0.1);
  pid1.setParameter(Signal::PIDController::Ki, 0.5);
  pid1.setParameter(Signal::PIDController::Kd, 0.01);

  scvs1.setParameter(Signal::SignalControlledVoltageSource::Gain, 0.001);

  mdc1.setParameter(Mna::MotorDC::L, 0.29366e-6); // Inductance
  mdc1.setParameter(Mna::MotorDC::R, 1.078); // Resistance
  mdc1.setParameter(Mna::MotorDC::B, 6.445e-7); // Damping coeficient
  mdc1.setParameter(Mna::MotorDC::K, 0.0022); // Motor gain
  mdc1.setParameter(Mna::MotorDC::J, 2.1518e-6); // Inertia

  system.connect(scvs1.getPort(Signal::SignalControlledVoltageSource::Positive), mdc1.getPort(Mna::MotorDC::Positive));
  system.connect(mdc1.getPort(Mna::MotorDC::Negative), g1.getPort(Mna::Ground::Reference));
  system.connect(g1.getPort(Mna::Ground::Reference), scvs1.getPort(Signal::SignalControlledVoltageSource::Negative));
  system.connect(mdc1.getPort(Mna::MotorDC::Velocity), sensor.getPort(Signal::VelocitySensor::Sensor));
  
  ofstream outputFile("output.csv");
  ostream& outputStream = cout;
  outputStream << "Time,PID Output,V source,Velocity Motor" << endl;
  for(double time = 0.0; time <= FINAL_TIME; time += TIME_STEP)
  {
	co1.step(TIME_STEP, time);
	  
	sd1.setInputPortValue(Signal::SumDiff::Reference, co1.getOutPortValue(Signal::ConstantOutput::Constant));
	sd1.setInputPortValue(Signal::SumDiff::Signal, sensor.getOutPortValue(Signal::VelocitySensor::Velocity));
	sd1.step(TIME_STEP, time);

	pid1.setInputPortValue(Signal::PIDController::Error, sd1.getOutPortValue(Signal::SumDiff::Difference));
	pid1.step(TIME_STEP, time);

	scvs1.setInputPortValue(Signal::SignalControlledVoltageSource::Signal, pid1.getOutPortValue(Signal::PIDController::CorrectedOutput));

	system.step(TIME_STEP, time);

    outputStream << time << ','
				 << pid1.getOutPortValue(Signal::PIDController::CorrectedOutput) << ','
                 << scvs1.getAcross(Signal::SignalControlledVoltageSource::Positive) << ','
                 << sensor.getOutPortValue(Signal::VelocitySensor::Velocity)
                 << endl;
  }

  return OK;
}
