#ifndef FORWARDANDBACKWARDRATES_HPP
#define FORWARDANDBACKWARDRATES_HPP

#include "Rate.hpp"

namespace BER {

  class ForwardAndBackwardRates {

    public:

      // Constructors
      ForwardAndBackwardRates();
      ForwardAndBackwardRates(Rate iForwardRate, 
                              Rate iBackwardRate);

      // Getters
      Rate getForwardRate();
      Rate getBackwardRate();
   
    private:

      Rate _forwardRate;
      Rate _backwardRate;

  };
}

#endif
