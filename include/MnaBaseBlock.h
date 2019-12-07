#pragma once
#include "Matrix.h"
#include "MnaPort.h"
#include "SimulationBlock.h"
#include <vector>

namespace ELCT350
{
  /**
   * @class MnaBaseBlock
   * @brief Base class for Mna block components
   * @details
   * This class is the base class for operations common to all mna components
   **/
  template<typename T>
  class MnaBaseBlock : public virtual SimulationBlock
  {
  public:
    #pragma region Destructor
    /**
     * Destructor releases resources upon deletion of instance
     */
    virtual ~MnaBaseBlock()
    {
      delete _acrossValues;
      delete _throughValues;
      delete _interceptVector;
      delete _jacobian;
    }
    #pragma endregion

    #pragma region Observers
    /**
     * Gets the jacobian value at the specified cell
     * @param[in] row the row index
     * @param[in] column the column index
     * @return the value at the specified cell of the jacobian
     */
    T getJacobian(size_t row, size_t column) const
    {
      return (*_jacobian)(row, column);
    }

    /**
     * Gets the intercept value at the specified row
     * @param[in] row the row index
     * @return the value of the intercept at the specified row index
     */
    T getIntercept(size_t row) const
    {
      return (*_interceptVector)(row, 0);
    }

    /**
     * @return the number of ports in the block
     */
    size_t getNumPorts() const
    {
      return _ports.size();
    }

    /**
     * Gets a specified port (const version)
     * @param[in] index the index of the port
     * @return the const port reference at the given index
     **/
    const MnaPort<T>& getPort(size_t index) const
    {
      return _ports.at(index);
    }

    /**
     * Gets a specified port
     * @param[in] index the index of the port
     * @return the port at the given index
     **/
    MnaPort<T>& getPort(size_t index)
    {
      return _ports[index];
    }

    /**
     * @param[in] row the row index
     * @return the across value at the specified row
     **/
    T getAcross(size_t row) const
    {
      return (*_acrossValues)(row, 0);
    }

    /**
     * @param[in] row the row index
     * @return the through value at the specified row
     **/
    T getThrough(size_t row) const
    {
      return (*_throughValues)(row, 0);
    }
    #pragma endregion

    #pragma region Modifiers
    /**
     * Sets the across value for a port
     * @param[in] row the row index
     * @param[in] across the new across value
     **/
    void setAcross(size_t row, T across)
    {
      (*_acrossValues)(row, 0) = across;
    }

    /**
     * Updates the through values once across values have been set
     **/
    void updateThrough()
    {
      for (const auto& rowPort : _ports)
      {
        (*_throughValues)(rowPort.Index, 0) = -(*_interceptVector)(rowPort.Index, 0);
        for (const auto& columnPort : _ports)
        {
          (*_throughValues)(rowPort.Index, 0) +=
            (*_jacobian)(rowPort.Index, columnPort.Index) * (*_acrossValues)(columnPort.Index, 0);
        }
      }
    }

    /**
     * The function that specifies the new jacobian and intercept stamps
     * @param[in] timeStep the time-step of the simulation
     * @param[in] currentTime the current time of the simulation
     **/
    virtual void step(double timeStep, double currentTime) = 0;

    virtual void postStep()
    {
    }
    #pragma endregion
  protected:
    #pragma region Constructor
    /**
     * Constructor for MnaBaseBlock. Initializes internal values.
     * @param[in] numParameters the number of parameters for the block
     * @param[in] numPorts the number of ports for the block
     **/
    MnaBaseBlock(size_t numPorts)
    {
      for (size_t portIndex = 0; portIndex < numPorts; ++portIndex)
        _ports.push_back({ portIndex, *this, false, false, 0 });

      _acrossValues = new Matrix<T>(_ports.size(), 1);
      _throughValues = new Matrix<T>(_ports.size(), 1);
      _interceptVector = new Matrix<T>(_ports.size(), 1);
      _jacobian = new Matrix<T>(_ports.size(), _ports.size());
    }
    #pragma endregion

    #pragma region Modifiers
    /**
     * Sets the value of a cell in the jacobian matrix
     * @param[in] row the row index
     * @param[in] column the column index
     * @param[in] value the new value of the cell at the specified index
     **/
    void setJacobian(size_t row, size_t column, T value)
    {
      (*_jacobian)(row, column) = value;
    }

    /**
     * Sets the value of a row in the intercept vector
     * @param[in] row the row index
     * @param[in] value the new value of the intercept at the specified row
     **/
    void setIntercept(size_t row, T value)
    {
      (*_interceptVector)(row, 0) = value;
    }
    #pragma endregion
  private:
#   pragma region Variables
    Matrix<T>* _acrossValues;
    Matrix<T>* _throughValues;
    Matrix<T>* _interceptVector;
    Matrix<T>* _jacobian;
    std::vector< MnaPort<T> > _ports;
    #pragma endregion
  };
}