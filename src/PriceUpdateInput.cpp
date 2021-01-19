#include "PriceUpdateInput.hpp"

#include <iostream>
#include <string>
#include <sstream>

namespace BER {

  // //////////////////////////////////////////////////////////////////////////
  PriceUpdateInput::PriceUpdateInput(const Timestamp & iTimestamp,
                                     const CurrencyOnExchange & iSourceCurrencyOnExchange,
                                     const CurrencyOnExchange & iDestinationCurrencyOnExchange,
                                     const Rate & iForwardRate,
                                     const Rate & iBackwardRate) 
   : _timestamp(iTimestamp),
     _sourceCurrencyOnExchange(iSourceCurrencyOnExchange),
     _destinationCurrencyOnExchange(iDestinationCurrencyOnExchange),
     _forwardRate(iForwardRate),
     _backwardRate(iBackwardRate) {
  }
  
  // ////////////////////////////////////////////////////////////////////////////
  std::string PriceUpdateInput::toString() const {

    std::stringstream ss;
    ss << "Price Update request\n" 
       << "From exchange " << _sourceCurrencyOnExchange.toString() << " to " << _destinationCurrencyOnExchange.toString() << "\n"
       << "From price " <<_forwardRate << " to " << _backwardRate << "\n"
       << "At timestamp " << _timestamp << "\n";
    return ss.str();
  } 

  // //////////////////////////////////////////////////////////////////////////////
  void PriceUpdateInput::execute() {

    // Call the price update on the graph    
    _exchangeRateGraph->updateRates(_sourceCurrencyOnExchange,
                                    _destinationCurrencyOnExchange,
                                    _forwardRate,
                                    _backwardRate,
                                    _timestamp);
 
  }

}

