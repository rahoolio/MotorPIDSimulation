#pragma once
#include <exception>
#include <string>

namespace ELCT350
{
  class SimulatorException : public std::exception
  {
    public:
      SimulatorException(const std::string& message) noexcept;
      virtual ~SimulatorException(){};

      virtual const char* what() const noexcept override;
    private:
      std::string _message;
  };
}
