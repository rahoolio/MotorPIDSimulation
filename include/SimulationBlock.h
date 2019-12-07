#pragma once
#include <vector>

namespace ELCT350
{
  class SimulationBlock
  {
  public:
    void setParameter(size_t index, double value);

  protected:
    SimulationBlock(size_t numParameters);

    std::vector<double> _parameters;
  };
}