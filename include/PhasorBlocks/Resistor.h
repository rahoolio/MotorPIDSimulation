#pragma once
#include "PhasorBlock.h"

namespace ELCT350
{
  namespace Phasor
  {
    class Resistor final : public PhasorBlock
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
        Resistance
      };
      #pragma endregion

      #pragma region Constructors and Destructors
      Resistor();
      #pragma endregion

      #pragma region PhasorBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      #pragma endregion
    };
  }
}