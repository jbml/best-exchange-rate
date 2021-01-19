#ifndef RATEANDTIMESTAMP_HPP
#define RATEANDTIMESTAMP_HPP

#include "Timestamp.hpp"
#include "Rate.hpp"

namespace BER {

  class RateAndTimestamp {

    public:

      // Constructors
      RateAndTimestamp();
      RateAndTimestamp(Rate iRate, 
                       const Timestamp & iTimestamp);

      // Getters
      Rate getRate() const;
      Timestamp getTimestamp() const;

    private:
  
      Rate _rate;
      Timestamp _timestamp;

  };
}

#endif
