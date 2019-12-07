#pragma once
#include "MnaBaseBlock.h"
#include <map>
#include <set>

namespace ELCT350
{
  double fabs(const std::complex<double>& value)
  {
    return sqrt(value * value).real();
  }

  /**
   * @class MnaBaseSystem
   * @brief Object for an system of MNA blocks
   * @details
   * This class provides an implementation of an MNA solver that keeps tracks of all MNA blocks
   * and how they are connected, steps them, composes the system matrices representing the nodal equations
   * and solves for system / updates MNA block values
   **/
  template<typename T>
  class MnaBaseSystem
  {
  public:
    #pragma region Constructors and Destructors
    /**
     * Constructor for MNA System, initializes internal values
     **/
    MnaBaseSystem()
      : _systemJacobian(nullptr), _systemIntercept(nullptr), _systemAcross(nullptr)
    {
      reinit();
    }

    /**
     * Destructor for MNA System, releases resources once instance is destroyed
     **/
    ~MnaBaseSystem()
    {
      if (_systemJacobian)
        delete _systemJacobian;
      if (_systemIntercept)
        delete _systemIntercept;
      if (_systemAcross)
        delete _systemAcross;
    }
    #pragma endregion

    #pragma region Modifiers
    /**
     * Called when simulation should be restarted
     **/
    void reinit()
    {
      for (auto* engine : _engines)
      {
        for (size_t port = 0; port < engine->getNumPorts(); ++port)
        {
          engine->getPort(port).AssignedNode = false;
        }
      }
      _engines.clear();
      _maxNodes = 0;
      _initialized = false;
    }

    /**
     * Connects two ports in the same node
     * @param[in] port1 a port to be connected
     * @param[in] port2 a port to be connected
     **/
    void connect(MnaPort<T>& port1, MnaPort<T>& port2)
    {
      if (port1.AssignedNode)
      {
        if (port2.AssignedNode)
        {
          //if they're already in the same node, we don't need to do anything
          if (port1.Node != port2.Node)
          {
            //otherwise, we need to merge the nodes. We'll put all the ports in the node of port2 into
            //the node of port 1
            for (auto* port : _nodes.at(port2.Node))
            {
              port->Node = port1.Node;
            }
          }
        }
        else
        {
          //put port 2 in port 1's node.
          port2.Node = port1.Node;
          port2.AssignedNode = true;
          _engines.emplace(&port2.Engine);
        }
      }
      else
      {
        if (port2.AssignedNode)
        {
          //put port1 in port2's node
          port1.Node = port2.Node;
          port1.AssignedNode = true;
          _engines.emplace(&port1.Engine);
        }
        else
        {
          //Put both port1 and port 2 in the same new node
          port1.Node = ++_maxNodes;
          port2.Node = _maxNodes;
          port1.AssignedNode = true;
          port2.AssignedNode = true;
          _engines.emplace(&port1.Engine);
          _engines.emplace(&port2.Engine);
        }
      }
    }

    /**
     * Peforms one step of the entire MNA system
     * @param[in] timeStep the time-step of the simulation
     * @param[in] currentTime the current time of the simulation
     **/
    void step(double timeStep, double currentTime)
    {
      if (!_initialized)
      {
        init();
      }

      stepEngines(timeStep, currentTime);
      copyJacobian(*_systemJacobian);
      copyIntercept(*_systemIntercept);
      solve(*_systemJacobian, *_systemIntercept, *_systemAcross);
      propagateValues(*_systemAcross);

      double mismatch = numeric_limits<double>::max();
      while (mismatch > 1e-7)
      {
        stepEngines(timeStep, currentTime);
        copyJacobian(*_systemJacobian);
        copyIntercept(*_systemIntercept);
        mismatch = newtonRaphsonSolve(*_systemJacobian, *_systemIntercept, *_systemAcross);
        propagateValues(*_systemAcross);
      }

      postStepEngines();
    }
    #pragma endregion

  private:
    #pragma region Variables
    Matrix<T>* _systemJacobian;
    Matrix<T>* _systemIntercept;
    Matrix<T>* _systemAcross;
    size_t _maxNodes;
    bool _initialized;
    std::vector< std::vector<MnaPort<T>*> > _nodes;
    std::set<MnaBaseBlock<T>*> _engines;
    #pragma endregion

    #pragma region Modifiers
    /**
     * Internal initialization of system and nodes
     **/
    void init()
    {
      std::map<size_t, std::vector<MnaPort<T>*> > nodes;
      //Add all ports to nodes
      for (auto* engine : _engines)
      {
        //add all the ports to their respective nodes
        for (size_t portIndex = 0; portIndex < engine->getNumPorts(); ++portIndex)
        {
          auto& port = engine->getPort(portIndex);
          if (!port.AssignedNode)
          {
            port.Node = ++_maxNodes;
            port.AssignedNode = true;
          }
          nodes[port.Node].push_back(&port);
        }
      }

      //Now we need to remove any reference nodes before adding them to our node list
      _nodes.clear();
      for (auto& portList : nodes)
      {
        bool isReference = false;
        size_t nodeIndex = _nodes.size();
        for (auto* port : portList.second)
        {
          if (port->Reference)
          {
            isReference = true;
            break;
          }
          port->Node = nodeIndex; //change the node number
        }

        //If any port is a reference, we don't need to add the node to our node list
        if (isReference)
        {
          for (auto port : portList.second)
            port->AssignedNode = false;
        }
        else
        {
          _nodes.push_back(move(portList.second));
        }
      }

      if (_systemJacobian)
        delete _systemJacobian;
      if (_systemIntercept)
        delete _systemIntercept;
      if (_systemAcross)
        delete _systemAcross;

      _systemJacobian = new Matrix<T>(_nodes.size(), _nodes.size());
      _systemIntercept = new Matrix<T>(_nodes.size(), 1);
      _systemAcross = new Matrix<T>(_nodes.size(), 1);

      _initialized = true;
    }

    /**
     * Calls step on all the engines in this system
     * @param[in] timeStep the time-step of the simulation
     * @param[in] currentTime the current time of the simulation
     **/
    void stepEngines(double timeStep, double currentTime)
    {
      for (auto* engine : _engines)
      {
        engine->step(timeStep, currentTime);
      }
    }

    /**
     * Calls step on all the engines in this system
     * @param[in] timeStep the time-step of the simulation
     * @param[in] currentTime the current time of the simulation
     **/
    void postStepEngines()
    {
      for (auto* engine : _engines)
      {
        engine->postStep();
      }
    }

    /**
     * Copies the jacobian from the MNA blocks to form the system jacobian
     * @param[out] jacobian a reference to the system jacobian
     **/
    void copyJacobian(Matrix<T>& jacobian)
    {
      jacobian.zeroMatrix();
      #pragma region Copy Jacobian from Engines into system here
      #pragma endregion
	  for (const auto* engine : _engines)
		  for (size_t row = 0; row < engine->getNumPorts(); row++)
		  {
			  const auto& rowPort = engine->getPort(row);
			  if (rowPort.AssignedNode)
			  {
				  for (size_t column = 0; column < engine->getNumPorts(); column++)
				  {
					  const auto& columnPort = engine->getPort(column);
					  if (columnPort.AssignedNode)
					  {
						  jacobian(rowPort.Node, columnPort.Node) += engine->getJacobian(row,column);
					  }
				  }
			  }
		  }
    }

    /**
     * Copies the intercept vector from the MNA blocks to the system intercept
     * @param[out] intercept a reference to the system intercept
     **/
    void copyIntercept(Matrix<T>& intercept)
    {
      intercept.zeroMatrix();
      #pragma region Copy Intercept from Engines into system here
      for (const auto* engine : _engines)
        for (size_t row = 0; row < engine->getNumPorts(); ++row)
        {
          const auto& rowPort = engine->getPort(row);
          if (rowPort.AssignedNode)
            intercept(rowPort.Node, 0) += engine->getIntercept(row);
        }
      #pragma endregion
    }

    /**
     * Solves for the across value given system jacobian and intercept
     * KCL: (0 = Jacobian * Across - Intercept), solving for Across
     * @param[in] jacobian the system jacobian matrix
     * @param[in] intercept the system intercept vector
     * @param[out] across the across vector solution
     **/
    void solve(const Matrix<T>& jacobian, const Matrix<T>& intercept, Matrix<T>& across)
    {
      #pragma region Solve for across here
      #pragma endregion

		across = jacobian.getInverse() * intercept;
    }

    /**
     * Solves for the across value given system jacobian and intercept
     * KCL: (0 = Jacobian * Across - Intercept), solving for Across
     * @param[in] jacobian the system jacobian matrix
     * @param[in] intercept the system intercept vector
     * @param[out] across the across vector solution
     **/
    double newtonRaphsonSolve(const Matrix<T>& jacobian, const Matrix<T>& intercept, Matrix<T>& across)
    {
      auto current = jacobian * across - intercept;
      T mismatch = 0.0;
      for (size_t row = 0; row < current.getRows(); ++row)
      {
        mismatch += current(row, 0);
      }

      #pragma region Solve for across here

      #pragma endregion

      return fabs(mismatch);
    }

    /**
     * Propagates the results back to the MNA blocks
     * (Sets across values and calls updateThrough)
     * @param[in] across the system across vector
     **/
    void propagateValues(const Matrix<T>& across)
    {
      #pragma region Propagate Across values to engines here

		for (auto* engine : _engines)
			for (size_t row = 0; row < engine->getNumPorts(); row++)
			{
				const auto& rowPort = engine->getPort(row);
				if (rowPort.AssignedNode)
				{
					engine->setAcross(row, across(rowPort.Node, 0));
				}
				else
				{
					engine->setAcross(row, 0);
				}
			}

      #pragma endregion

      for (auto* engine : _engines)
      {
        engine->updateThrough();
      }
    }
    #pragma endregion
  };
}