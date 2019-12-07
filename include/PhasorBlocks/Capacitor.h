#pragma once
#include "PhasorBlock.h"

namespace ELCT350
{
  namespace Phasor
  {
    class Capacitor final : public PhasorBlock
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

      #pragma region PhasorBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      #pragma endregion
    };
  }
}