#include "RateAndTimestamp.hpp"

namespace BER {

  // /////////////////////////////////////////////////////////////////////
  RateAndTimestamp::RateAndTimestamp()
   : _rate(0), _timestamp(0) {
  }

  // /////////////////////////////////////////////////////////////////////
  RateAndTimestamp::RateAndTimestamp(Rate iRate, 
                                     const Timestamp & iTimestamp)
   : _rate(iRate), _timestamp(iTimestamp) {
  }  

  // /////////////////////////////////////////////////////////////////////
  Rate RateAndTimestamp::getRate() const {
    return _rate;
  }

  // /////////////////////////////////////////////////////////////////////
  Timestamp RateAndTimestamp::getTimestamp() const {
    return _timestamp;
  }

}
