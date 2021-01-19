#ifndef INPUTREQUESTFACTORY_HPP
#define INPUTREQUESTFACTORY_HPP

#include <string>

#include "InputRequest.hpp"

namespace BER {

  class InputRequestFactory {
 
    public:
 
      // Validate, build and return a request, based on the raw string 
      InputRequest * buildInputRequest(const std::string & iRawInputRequest);

      // Clean memory of created input requests
      bool clean();

    private:

      // Check the validity of the rate request, as split per token
      bool checkRateRequestValidity(const std::vector<std::string> & iTokens,
                                    CurrencyOnExchange & oSourceCurrencyOnExchange,
                                    CurrencyOnExchange & oDestinationCurrencyOnExchange);

      // Check the validity of the price update request, as a split token
      bool checkPriceUpdateValidity(const std::vector<std::string> & iTokens,
                                    Timestamp & oTimestamp,
                                    CurrencyOnExchange & oSourceCurrencyOnExchange,
                                    CurrencyOnExchange & oDestinationCurrencyOnExchange,
                                    Rate & oForwardRate,
                                    Rate & oBackwardRate);

      // Helper function for checking timestamps
      bool checkTimestampValidity(const std::string & iTimestampString,
                                  Timestamp & oTimestamp);

      // List of created input requests
      std::list <InputRequest * > _inputRequests;

  };
}

#endif
