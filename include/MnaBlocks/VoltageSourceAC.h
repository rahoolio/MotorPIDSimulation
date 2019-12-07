#pragma once
#include "MnaBlock.h"

namespace ELCT350
{
  namespace Mna
  {
    class VoltageSourceAC final : public MnaBlock
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
        Phase,
        Offset
      };
      #pragma endregion

      #pragma region Constructors and Destructors
      VoltageSourceAC();
      #pragma endregion

      #pragma region MnaBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      #pragma endregion
    };
  }
}