#include "CurrencyOnExchange.hpp"

namespace BER {

  // //////////////////////////////////////////////////////////////////////////
  CurrencyOnExchange::CurrencyOnExchange() {
  }
 
  // //////////////////////////////////////////////////////////////////////////
  CurrencyOnExchange::CurrencyOnExchange(const std::string & iCurrencyCode, 
                                         const std::string & iExchangeCode) 
   : _currencyCode(iCurrencyCode), _exchangeCode(iExchangeCode) {
  }

  // //////////////////////////////////////////////////////////////////////////
  bool CurrencyOnExchange::operator<(const CurrencyOnExchange & iOtherCurrencyOnExchange) const {

   // Exchange code comparison first   
   if(_exchangeCode < iOtherCurrencyOnExchange.getExchangeCode()) {
      return true;
    } 
    
    if(_exchangeCode == iOtherCurrencyOnExchange.getExchangeCode()) {
      // Currency code comparison second
      if(_currencyCode < iOtherCurrencyOnExchange.getCurrencyCode()) {
        return true;
      }
    }

    return false;
  }
  
  // //////////////////////////////////////////////////////////////////////////
  bool CurrencyOnExchange::operator==(const CurrencyOnExchange & iOtherCurrencyOnExchange) const {
   return(_exchangeCode == iOtherCurrencyOnExchange.getExchangeCode() &&
          _currencyCode == iOtherCurrencyOnExchange.getCurrencyCode());
  }

  // //////////////////////////////////////////////////////////////////////////
  std::string CurrencyOnExchange::getCurrencyCode() const {
    return _currencyCode;
  }

  // //////////////////////////////////////////////////////////////////////////
  std::string CurrencyOnExchange::getExchangeCode() const {
    return _exchangeCode;
  }

  // //////////////////////////////////////////////////////////////////////////  
  void CurrencyOnExchange::setCurrencyCode(const std::string & iCurrencyCode) {
    _currencyCode = iCurrencyCode;
  }

  // //////////////////////////////////////////////////////////////////////////
  void CurrencyOnExchange::setExchangeCode(const std::string & iExchangeCode) {
    _exchangeCode = iExchangeCode;
  }
    
  // //////////////////////////////////////////////////////////////////////////
  std::string CurrencyOnExchange::toString() const {
    return "(" + _exchangeCode + "," + _currencyCode + ")";
  }
  
}

