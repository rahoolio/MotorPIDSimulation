#pragma once
#include "SimulationBlock.h"

namespace ELCT350
{
  class SignalBlock : public virtual SimulationBlock
  {
  public:
    double getOutPortValue(size_t index) const;
    void setInputPortValue(size_t index, double value);

  protected:
    SignalBlock(size_t numInputs, size_t numOutputs);

    virtual void step(double timeStep, double currentTime) = 0;

    std::vector<double> _outputs;
    std::vector<double> _inputs;
  };
}