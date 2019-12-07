#pragma once
#include "MnaBaseBlock.h"

namespace ELCT350
{
  /**
   * @class MnaBlock
   * @brief Base class for Mna block components
   * @details
   * This class is the base class for operations common to all mna components
   **/
  class MnaBlock : public MnaBaseBlock<double>
  {
  public:
    #pragma region Constructions
    MnaBlock(size_t numMnaPorts);
    #pragma endregion
  };
}