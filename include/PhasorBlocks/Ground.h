#pragma once
#include "PhasorBlock.h"

namespace ELCT350
{
  namespace Phasor
  {
    class Ground final : public PhasorBlock
    {
    public:
      #pragma region Enumerations
      enum Ports : size_t
      {
        Reference
      };
      #pragma endregion

      #pragma region Constructors and Destructors
      Ground();
      #pragma endregion

      #pragma region PhasorBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      #pragma endregion
    };
  }
}