#ifndef PRICEUPDATEINPUT_HPP
#define PRICEUPDATEINPUT_HPP 

#include "InputRequest.hpp"
#include "CurrencyOnExchange.hpp"
#include "Timestamp.hpp"
#include "Rate.hpp"

namespace BER {

  class PriceUpdateInput : public InputRequest {

    public:

      // Constructor
      PriceUpdateInput(const Timestamp & iTimestamp,
                       const CurrencyOnExchange & iSourceCurrencyOnExchange,
                       const CurrencyOnExchange & iDestinationCurrencyOnExchange,
                       const Rate & iForwardRate,
                       const Rate & iBackwardRate);

      // Perform the price update request
      void execute();
   
      // Return a price update request as a string
      std::string toString() const;

    private:
   
      Timestamp  _timestamp;
      CurrencyOnExchange _sourceCurrencyOnExchange;
      CurrencyOnExchange _destinationCurrencyOnExchange;
      Rate _forwardRate;
      Rate _backwardRate;

  };

}

#endif
