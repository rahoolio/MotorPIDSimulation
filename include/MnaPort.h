#pragma once

namespace ELCT350
{
  template<typename T> class MnaBaseBlock;

  /**
   * @class MnaPort
   * @brief Port object for MnaBlocks
   * @details
   * This struct holds Mna port properties
   **/
  template<typename T>
  struct MnaPort
  {
    size_t Index; //port index
    MnaBaseBlock<T>& Engine; //reference to port owner
    bool Reference; //indicates whether port is reference
    bool AssignedNode; //indicates whether system has assigned a node to this port
    size_t Node; //node index in system
  };
}