#pragma once
#include "MnaBaseSystem.h"
#include "MnaBlock.h"

namespace ELCT350
{
  /**
   * @class MnaSystem
   * @brief Object for an system of MNA blocks
   * @details
   * This class provides an implementation of an MNA solver that keeps tracks of all MNA blocks
   * and how they are connected, steps them, composes the system matrices representing the nodal equations
   * and solves for system / updates MNA block values
   **/
  class MnaSystem final : public MnaBaseSystem<double>
  {
  };
}