#pragma once
#include "MnaBlock.h"

namespace ELCT350
{
  namespace Mna
  {
    class Capacitor final : public MnaBlock
    {
    public:
      #pragma region Enumerations
      enum Ports : size_t
      {
        Negative,
        Positive
      };

      enum Parameters : size_t
      {
        Capacitance
      };
      #pragma endregion

      #pragma region Constructors and Destructors
      Capacitor();
      #pragma endregion

      #pragma region MnaBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      virtual void postStep() override;
      #pragma endregion
    private:
      #pragma region variables
      double _voltage;
      #pragma endregion
    };
  }
}