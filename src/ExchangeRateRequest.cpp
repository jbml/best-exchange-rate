#include "InputRequest.hpp"
#include "CurrencyOnExchange.hpp"
#include "ExchangeRateRequest.hpp"

#include <sstream>
#include <iostream>

namespace BER {

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  ExchangeRateRequest::ExchangeRateRequest(const CurrencyOnExchange & iSourceCurrencyOnExchange,
                                           const CurrencyOnExchange & iDestinationCurrencyOnExchange)
   : _sourceCurrencyOnExchange(iSourceCurrencyOnExchange),
     _destinationCurrencyOnExchange(iDestinationCurrencyOnExchange) {
  }

  // //////////////////////////////////////////////////////////////////////////////////////////////////
  CurrencyOnExchange ExchangeRateRequest::getSourceCurrencyOnExchange() const  {
    return _sourceCurrencyOnExchange;
  }
 
  // //////////////////////////////////////////////////////////////////////////////////////////////////
  CurrencyOnExchange ExchangeRateRequest::getDestinationCurrencyOnExchange() const  {
    return _destinationCurrencyOnExchange;
  }
 
  // /////////////////////////////////////////////////////////////////////////////////////////////////// 
  std::string ExchangeRateRequest::toString() const {
    std::stringstream ss;
    ss << "Exhange Rate request\n"
       << "From exchange " << _sourceCurrencyOnExchange.toString() << " to " << _destinationCurrencyOnExchange.toString() << "\n";
    return ss.str();
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  bool ExchangeRateRequest::operator==(const ExchangeRateRequest & iOtherExchangeRateRequest) const {
    return (_sourceCurrencyOnExchange == iOtherExchangeRateRequest.getSourceCurrencyOnExchange() &&
            _destinationCurrencyOnExchange < iOtherExchangeRateRequest.getDestinationCurrencyOnExchange());
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  bool ExchangeRateRequest::operator<(const ExchangeRateRequest & iOtherExchangeRateRequest) const {

    // Source comparison first
    if(_sourceCurrencyOnExchange < iOtherExchangeRateRequest.getSourceCurrencyOnExchange()) {
      return true;
    }

    if(_sourceCurrencyOnExchange == iOtherExchangeRateRequest.getSourceCurrencyOnExchange()) {
      // Destination comparison second
      if(_destinationCurrencyOnExchange < iOtherExchangeRateRequest.getDestinationCurrencyOnExchange()) {
        return true;
      }
    }

    return false;

  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  void ExchangeRateRequest::execute() {

    Rate aBestExchangeRate;
    std::list< CurrencyOnExchange > aPathForBestExchangeRate;
    bool aInfiniteCycleFlag;

    // Get the best echange rate from the graph
    _exchangeRateGraph->findBestExchangeRate(_sourceCurrencyOnExchange,
                                             _destinationCurrencyOnExchange,
                                             aBestExchangeRate,
                                             aPathForBestExchangeRate,
                                             aInfiniteCycleFlag);

    // In case there is an infinite cycle in the graph
    if(aInfiniteCycleFlag == true) {

      // If the same request is present in the cache, recalculate the current rate
      if(_replyCache.find(*this) != _replyCache.end()) {
        //std::cout << "Using cached path" << std::endl;
        _exchangeRateGraph->calculateRateFromPath(_replyCache[*this],
                                                  aBestExchangeRate);

        std::cout << "BEST_RATES_BEGIN " << _sourceCurrencyOnExchange.getExchangeCode() << " " << _sourceCurrencyOnExchange.getCurrencyCode() << " "
                                         << _destinationCurrencyOnExchange.getExchangeCode() << " " << _destinationCurrencyOnExchange.getCurrencyCode() << " ";
        std::cout << aBestExchangeRate << std::endl;
        for(std::list< CurrencyOnExchange >::const_iterator itCachedPath = _replyCache[*this].begin();
            itCachedPath != _replyCache[*this].end();
            ++itCachedPath) {
          std::cout << itCachedPath->getExchangeCode() << ", " << itCachedPath->getCurrencyCode() << std::endl;
        }
        std::cout << "BEST_RATES_END (CACHED)" << std::endl;
        return;
      }
      else {
        // Otherwise return an error
        std::cout << "BEST_RATES_BEGIN " << _sourceCurrencyOnExchange.getExchangeCode() << " " << _sourceCurrencyOnExchange.getCurrencyCode() << " "
                                         << _destinationCurrencyOnExchange.getExchangeCode() << " " << _destinationCurrencyOnExchange.getCurrencyCode() << " ";
        std::cout << "CYCLE" << std::endl;
        std::cout << "BEST_RATES_END" << std::endl;
        return;
      }
    }

    // In case there is no path possible
    if(aPathForBestExchangeRate.size() == 0) {
      std::cout << "BEST_RATES_BEGIN " << _sourceCurrencyOnExchange.getExchangeCode() << " " << _sourceCurrencyOnExchange.getCurrencyCode() << " "
                                       << _destinationCurrencyOnExchange.getExchangeCode() << " " << _destinationCurrencyOnExchange.getCurrencyCode() << " ";
      std::cout << "IMPOSSIBLE" << std::endl;
      std::cout << "BEST_RATES_END" << std::endl;
      return;
    }

    // BEST_RATES_BEGIN <source_exchange> <source_currency> <destination_exchange> <destination_currency> <rate>
    // <source_exchange, source_currency>
    // <exchange, currency>
    // <exchange, currency>
    // ...
    // <destination_exchange, destination_currency>
    // BEST_RATES_END
  
    std::cout << "BEST_RATES_BEGIN " << _sourceCurrencyOnExchange.getExchangeCode() << " " << _sourceCurrencyOnExchange.getCurrencyCode() << " "
                                     << _destinationCurrencyOnExchange.getExchangeCode() << " " << _destinationCurrencyOnExchange.getCurrencyCode() << " ";
    std::cout << aBestExchangeRate << std::endl;

    for(std::list< CurrencyOnExchange >::const_iterator itPath = aPathForBestExchangeRate.begin();
        itPath != aPathForBestExchangeRate.end();
        ++itPath) {
      std::cout << itPath->getExchangeCode() << ", " << itPath->getCurrencyCode() << std::endl;
    }
    std::cout << "BEST_RATES_END" << std::endl;

    // Store the solution in the cache
    _replyCache[*this] = aPathForBestExchangeRate;

    return;
  }

  std::map<ExchangeRateRequest, ExchangeRateReply> ExchangeRateRequest::_replyCache;

}

