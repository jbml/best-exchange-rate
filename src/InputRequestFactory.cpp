#include <string>
#include <regex>
#include <iostream>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include "InputRequestFactory.hpp"
#include "ExchangeRateRequest.hpp"
#include "PriceUpdateInput.hpp"

namespace BER {

  // ///////////////////////////////////////////////////////////////////////////////////////
  InputRequest * InputRequestFactory::buildInputRequest(const std::string & iRawInputRequest) {

    // Check if that's an empty line
    if(iRawInputRequest.size() == 0) {
      return NULL;
    }

    // Interpret the raw input request to determine its type
    std::istringstream aRawInputStream(iRawInputRequest);
    std::vector<std::string> aRawInputTokenizedVector((std::istream_iterator<std::string>(aRawInputStream)),
                                                       std::istream_iterator<std::string>());
   
    // Check if this is an EXCHANGE_RATE_REQUEST
    if(aRawInputTokenizedVector[0] == "EXCHANGE_RATE_REQUEST") {

      // Check that we have the right number of tokens with the right type
      CurrencyOnExchange aSourceCurrencyOnExchange;
      CurrencyOnExchange aDestinationCurrencyOnExchange;
      bool isValidRateRequest = checkRateRequestValidity(aRawInputTokenizedVector,
                                                         aSourceCurrencyOnExchange,
                                                         aDestinationCurrencyOnExchange);
      
      if(isValidRateRequest == true) {
        InputRequest * aRateRequestInput = new ExchangeRateRequest(aSourceCurrencyOnExchange,
                                                                   aDestinationCurrencyOnExchange);
        _inputRequests.push_back(aRateRequestInput);
        return aRateRequestInput;
      }
    }
    // Else, we check if is a price update
    else {
    
      CurrencyOnExchange aSourceCurrencyOnExchange;
      CurrencyOnExchange aDestinationCurrencyOnExchange;
      Timestamp aTimeStamp;
      Rate aForwardRate;
      Rate aBackwardRate; 
      bool isValidPriceUpdate = checkPriceUpdateValidity(aRawInputTokenizedVector,
                                                         aTimeStamp,
                                                         aSourceCurrencyOnExchange,
                                                         aDestinationCurrencyOnExchange,
                                                         aForwardRate,
                                                         aBackwardRate);
      if(isValidPriceUpdate == true) {
        InputRequest * aPriceUpdateInput = new PriceUpdateInput(aTimeStamp,
                                                                aSourceCurrencyOnExchange,
                                                                aDestinationCurrencyOnExchange,
                                                                aForwardRate,
                                                                aBackwardRate);
        _inputRequests.push_back(aPriceUpdateInput);
        return aPriceUpdateInput;
      }
      // Otherwise, it's an invalid input and we return NULL
      else {
        return NULL;
      }
    }

    return NULL;
  }

  // ////////////////////////////////////////////////////////////////////////////
  bool InputRequestFactory::clean() {
    for(std::list<InputRequest *>::iterator itFactory = _inputRequests.begin();
        itFactory != _inputRequests.end();
        ++itFactory) {
      delete *itFactory;
    }
    _inputRequests.clear();
  }

  // ////////////////////////////////////////////////////////////////////////////
  bool InputRequestFactory::checkRateRequestValidity(const std::vector<std::string> & iTokens,
                                                     CurrencyOnExchange & oSourceCurrencyOnExchange,
                                                     CurrencyOnExchange & oDestinationCurrencyOnExchange) {
   
    // Check the first token is the action code
    bool hasTheRightNumberOfInputs = (iTokens.size() == 5);
    if(hasTheRightNumberOfInputs == false) {
      return false;
    }
  
    // Double check the action code
    bool isActionCodeValid = (iTokens[0] == "EXCHANGE_RATE_REQUEST");
    if(isActionCodeValid == false) {
      return false;
    } 

    // Decode the parameters
    oSourceCurrencyOnExchange.setExchangeCode(iTokens[1]);
    oSourceCurrencyOnExchange.setCurrencyCode(iTokens[2]);
 
    oDestinationCurrencyOnExchange.setExchangeCode(iTokens[3]);
    oDestinationCurrencyOnExchange.setCurrencyCode(iTokens[4]);
   
    return true;
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////
  bool InputRequestFactory::checkPriceUpdateValidity(const std::vector<std::string> & iTokens,
                                                     Timestamp & oTimestamp,
                                                     CurrencyOnExchange & oSourceCurrencyOnExchange,
                                                     CurrencyOnExchange & oDestinationCurrencyOnExchange,
                                                     Rate & oForwardRate,
                                                     Rate & oBackwardRate) {

    // Check the number of inputs
    bool hasTheRightNumberOfInputs = (iTokens.size() == 6);
    if(hasTheRightNumberOfInputs == false) {
      return false;
    }

    // Decode the parameters, starting with the timestamp
    Timestamp aConvertedTimestamp;
    bool isTimestampValid = checkTimestampValidity(iTokens[0],
                                                  aConvertedTimestamp);
    
    if(isTimestampValid == false) {
      return false;
    }
    else {
      oTimestamp = aConvertedTimestamp;
    }

    // Decode the currencies and exchange codes
    oSourceCurrencyOnExchange.setExchangeCode(iTokens[1]);
    oSourceCurrencyOnExchange.setCurrencyCode(iTokens[2]);

    oDestinationCurrencyOnExchange.setExchangeCode(iTokens[1]);
    oDestinationCurrencyOnExchange.setCurrencyCode(iTokens[3]);

    // Decode the rates
    try {
      oForwardRate = strtod(iTokens[4].c_str(), NULL);
    }
    catch(...) {
      return false;
    }

    try {
      oBackwardRate = strtod(iTokens[5].c_str(), NULL);
    }
    catch(...) {
      return false;
    }

    // Check that none of forward or backward rate is 0
    if(oForwardRate == 0 || oBackwardRate == 0) {
      return false;
    }

    // Reject pairs where the forward*backward is more than 1
    if(oForwardRate * oBackwardRate > 1) {
      return false;
    }
   
    return true;

  }

  // ///////////////////////////////////////////////////////////////////////
  bool InputRequestFactory::checkTimestampValidity(const std::string & iTimestampString,
                                                   Timestamp & oTimestamp) {

    // 2017-11-01T09:42:23+00:00
   
    // Check the length of the string
    if(iTimestampString.size() != 25) {
      return false;
    }

    // Check the format of the timestamp
    bool isTimestampValid = true;
    // Year
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[0]);
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[1]);
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[2]);
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[3]);
    // Separator
    isTimestampValid = isTimestampValid && iTimestampString[4] == '-';
    // Month
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[5]);
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[6]);
    // Separator
    isTimestampValid = isTimestampValid && iTimestampString[7] == '-';
    // Day
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[8]);
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[9]);
    // Separator
    isTimestampValid = isTimestampValid && iTimestampString[10] == 'T';
    // Hour
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[11]);
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[12]);
    // Separator
    isTimestampValid = isTimestampValid && iTimestampString[13] == ':';
    // Minutes
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[14]);
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[15]);
    // Separator
    isTimestampValid = isTimestampValid && iTimestampString[16] == ':';
    // Seconds
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[17]);
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[18]);
    // Separator
    char aTimeZonePlusOrMinus = iTimestampString[19];
    isTimestampValid = isTimestampValid && (aTimeZonePlusOrMinus == '+' || aTimeZonePlusOrMinus == '-');
    // Timezone hour
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[20]);
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[21]);
    // Separator
    isTimestampValid = isTimestampValid && iTimestampString[22] == ':';
    // Timezone hour
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[23]);
    isTimestampValid = isTimestampValid && std::isdigit(iTimestampString[24]);
  
    if(isTimestampValid == false) {
      return false;
    }
 
    // Get the timestamp without the timezone adjustment
    std::string aTimestampWithoutTimezone = iTimestampString.substr(0, 19);
    struct tm aTimeStructure;
    char * aConversionOutput = strptime(aTimestampWithoutTimezone.c_str(), "%FT%T", &aTimeStructure);
 
    if(aConversionOutput == NULL) {
      return false;
    }
 
    // Adjust the timestamp based on the timezone
    int aTimeZoneHour = atoi(iTimestampString.substr(20,2).c_str());
    if(aTimeZoneHour < 0 || aTimeZoneHour > 23) {
      return false;
    }    

    int aTimeZoneMinute = atoi(iTimestampString.substr(23,2).c_str());
    if(aTimeZoneMinute < 0 || aTimeZoneMinute > 59) {
      return false;
    }

    if(aTimeZonePlusOrMinus == '+') {
      aTimeStructure.tm_hour -= aTimeZoneHour;
      aTimeStructure.tm_min -= aTimeZoneMinute;
    }
    else {
      aTimeStructure.tm_hour += aTimeZoneHour;
      aTimeStructure.tm_min += aTimeZoneMinute;
    }
 
    oTimestamp = mktime(&aTimeStructure);
    
    return true;
  }

}
