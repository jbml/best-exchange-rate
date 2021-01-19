#ifndef CURRENCYONEXCHANGE_HPP
#define CURRENCYONEXCHANGE_HPP

#include <utility>
#include <string>

namespace BER {

  class CurrencyOnExchange {

    public: 
     
      // Constructors 
      CurrencyOnExchange();
      CurrencyOnExchange(const std::string & iCurrencyCode, 
                         const std::string & iExchangeCode);

      // Comparison operators
      bool operator<(const CurrencyOnExchange & iOtherCurrencyOnExchange) const;
      bool operator==(const CurrencyOnExchange & iOtherCurrencyOnExchange) const;

      // Stream 
      std::string toString() const;

      // Getters
      std::string getCurrencyCode() const;
      std::string getExchangeCode() const;
     
      // Setters
      void setCurrencyCode(const std::string & iCurrencyCode);
      void setExchangeCode(const std::string & iExchangeCode);

    private:

      std::string _currencyCode;
      std::string _exchangeCode;

  };
}

#endif
