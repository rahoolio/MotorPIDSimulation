#pragma once
#include "MnaBlock.h"

namespace ELCT350
{
  namespace Mna
  {
    class VoltageSourceDC final : public MnaBlock
    {
    public:
      #pragma region Enumerations
      enum Ports : size_t
      {
        Negative,
        Positive,
        SupplyCurrent_Internal
      };

      enum Parameters : size_t
      {
        Voltage
      };
      #pragma endregion

      #pragma region Constructors and Destructors
      VoltageSourceDC();
      #pragma endregion

      #pragma region MnaBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      #pragma endregion
    };
  }
}