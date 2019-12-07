#pragma once
#include "MnaBaseBlock.h"
#include <complex>

namespace ELCT350
{
  /**
   * @class PhasorBlock
   * @brief Base class for Phasor block components
   * @details
   * This class is the base class for operations common to all phasor components
   **/
  class PhasorBlock : public MnaBaseBlock< std::complex<double> >
  {
  public:
    #pragma region Constructions
    PhasorBlock(size_t numMnaPorts);
    #pragma endregion
  };
}