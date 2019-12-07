#pragma once
#include "MnaBlock.h"
#include "SignalBlock.h"

namespace ELCT350
{
  namespace Signal
  {
    class VelocitySensor final : public MnaBlock, public SignalBlock
    {
    public:
      #pragma region Enumerations
      enum MnaPorts : size_t
      {
        Sensor
      };

      enum OutputSignalPorts : size_t
      {
        Velocity
      };
      #pragma endregion

      #pragma region Constructors and Destructors
      VelocitySensor();
      #pragma endregion

      #pragma region PhasorBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      #pragma endregion
    };
  }
}