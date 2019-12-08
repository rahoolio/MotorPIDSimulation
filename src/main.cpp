#include <iostream>
#include <fstream>
#include "Matrix.h"
#include "MnaSystem.h"
#include "MnaBlocks/Ground.h"
#include "MnaBlocks/Resistor.h"
#include "MnaBlocks/VoltageSourceAC.h"
#include "MnaBlocks/Capacitor.h"
#include "SignalBlocks/VelocitySensor.h"

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
  Mna::Ground g;
  Mna::VoltageSourceAC vac1, vac2;
  Mna::Resistor r1;
  Mna::Capacitor c1;
  Signal::VelocitySensor sensor;
  

  vac1.setParameter(Mna::VoltageSourceAC::Voltage, 4.0);
  vac1.setParameter(Mna::VoltageSourceAC::Frequency, 0.1);
  vac2.setParameter(Mna::VoltageSourceAC::Voltage, 4.0);
  vac2.setParameter(Mna::VoltageSourceAC::Frequency, 1.0);

  r1.setParameter(Mna::Resistor::Resistance, 1);
  c1.setParameter(Mna::Capacitor::Capacitance, 2);

  system.connect(vac1.getPort(Mna::VoltageSourceAC::Positive), vac2.getPort(Mna::VoltageSourceAC::Negative));
  system.connect(vac2.getPort(Mna::VoltageSourceAC::Positive), r1.getPort(Mna::Resistor::Positive));
  system.connect(r1.getPort(Mna::Resistor::Negative), c1.getPort(Mna::Capacitor::Positive));
  system.connect(c1.getPort(Mna::Capacitor::Negative), g.getPort(Mna::Ground::Reference));
  system.connect(g.getPort(Mna::Ground::Reference), vac1.getPort(Mna::VoltageSourceAC::Negative));
  
  system.connect(c1.getPort(Mna::Capacitor::Positive), sensor.getPort(Signal::VelocitySensor::Sensor));

  ofstream outputFile("output.csv");
  ostream& outputStream = outputFile;
  outputStream << "Time,V source,Voltage Capacitor" << endl;
  for(double time = 0.0; time <= FINAL_TIME; time += TIME_STEP)
  {
    system.step(TIME_STEP, time);
    outputStream << time << ','
                 << vac2.getAcross(Mna::VoltageSourceAC::Positive) << ','
                 << sensor.getOutPortValue(Signal::VelocitySensor::Velocity)
                 << endl;
  }

  return OK;
}
