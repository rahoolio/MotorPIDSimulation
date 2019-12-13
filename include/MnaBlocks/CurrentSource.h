#pragma once
#include "MnaBlock.h"

namespace ELCT350
{
  namespace Mna
  {
    class CurrentSource final : public MnaBlock
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
        Current
      };
      #pragma endregion

      #pragma region Constructors and Destructors
      CurrentSource();
      #pragma endregion

      #pragma region MnaBlock
      virtual void step(double timeStep, double dCurrentTime) override;
      #pragma endregion
    };
  }
}