#include "InputRequest.hpp"

namespace BER {

  // ///////////////////////////////////////////////////////////////// 
  void InputRequest::createLinkWithGraph(ExchangeRateGraph * ioExchangeRateGraph) {
    _exchangeRateGraph = ioExchangeRateGraph;    
  }

}
