#include "ForwardAndBackwardRates.hpp"

namespace BER {

  // ////////////////////////////////////////////////////////////////////
  ForwardAndBackwardRates::ForwardAndBackwardRates()
   : _forwardRate(0), _backwardRate(0) {
  }

  // ////////////////////////////////////////////////////////////////////
  ForwardAndBackwardRates::ForwardAndBackwardRates(Rate iForwardRate,
                                                   Rate iBackwardRate)
   : _forwardRate(iForwardRate), _backwardRate(iBackwardRate) {
  }
      
  // ////////////////////////////////////////////////////////////////////
  Rate ForwardAndBackwardRates::getForwardRate() {
    return _forwardRate;
  }
 
  // ////////////////////////////////////////////////////////////////////
  Rate ForwardAndBackwardRates::getBackwardRate() {
    return _backwardRate;
  }
   
}
