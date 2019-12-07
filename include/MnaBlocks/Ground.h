#pragma once
#include "MnaBlock.h"

namespace ELCT350
{
  namespace Mna
  {
    class Ground final : public MnaBlock
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

      #pragma region MnaBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      #pragma endregion
    };
  }
}