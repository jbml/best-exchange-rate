#ifndef EXCHANGERATREQUEST_HPP
#define EXCHANGERATREQUEST_HPP 

#include <map>

#include "InputRequest.hpp"
#include "CurrencyOnExchange.hpp"
#include "ExchangeRateReply.hpp"

namespace BER {

  class ExchangeRateRequest : public InputRequest {

    public:

      // Constructor
      ExchangeRateRequest(const CurrencyOnExchange & iSourceCurrencyOnExchange,
                          const CurrencyOnExchange & iDestinationCurrencyOnExchange);

      // Getters
      CurrencyOnExchange getSourceCurrencyOnExchange() const;
      CurrencyOnExchange getDestinationCurrencyOnExchange() const;

      // Stream
      std::string toString() const;

      // Compariosn operators
      bool operator<(const ExchangeRateRequest & iOtherExchangeRateRequest) const;
      bool operator==(const ExchangeRateRequest & iOtherExchangeRateRequest) const; 

      // Execute the exchange rate calcualation request
      void execute();

    private:
    
      CurrencyOnExchange _sourceCurrencyOnExchange;
      CurrencyOnExchange _destinationCurrencyOnExchange;
    
      static std::map<ExchangeRateRequest, ExchangeRateReply> _replyCache;
  };

}

#endif
