#pragma once
#include "PhasorBlock.h"

namespace ELCT350
{
  namespace Phasor
  {
    class VoltageSourceAC final : public PhasorBlock
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
        Voltage,
        Frequency,
        Phase
      };
      #pragma endregion

      #pragma region Constructors and Destructors
      VoltageSourceAC();
      #pragma endregion

      #pragma region PhasorBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      #pragma endregion
    };
  }
}