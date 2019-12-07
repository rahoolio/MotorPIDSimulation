#include "PhasorBlock.h"

using namespace ELCT350;

PhasorBlock::PhasorBlock(size_t numMnaPorts)
           : MnaBaseBlock< std::complex<double> >(numMnaPorts)
{
}