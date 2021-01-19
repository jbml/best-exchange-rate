#ifndef INPUTREQUEST_HPP
#define INPUTREQUEST_HPP

#include <string>

#include "ExchangeRateGraph.hpp"

namespace BER {

  // Abstract class to represent an input request received by the system
  class InputRequest {

    public:

      // Constructor 
      void createLinkWithGraph(ExchangeRateGraph * ioExchangeRateGraph);

      // Execute the required actions associated with the input request
      virtual void execute() = 0;
   
      // Convert to string
      virtual std::string toString() const = 0;

    protected:
      
      ExchangeRateGraph * _exchangeRateGraph;

  };
}

#endif
