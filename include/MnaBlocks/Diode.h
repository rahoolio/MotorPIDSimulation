#pragma once
#include "MnaBlock.h"

namespace ELCT350
{
  namespace Mna
  {
    class Diode final : public MnaBlock
    {
    public:
      #pragma region Enumerations
      enum Ports : size_t
      {
        Anode,
        Cathode
      };

      enum Parameters : size_t
      {
        SaturationCurrent,
        ThermalVoltage,
        IdealityFactor
      };
      #pragma endregion

      #pragma region Constructors and Destructors
      Diode();
      #pragma endregion

      #pragma region MnaBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      #pragma endregion
    };
  }
}