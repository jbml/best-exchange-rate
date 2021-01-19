#ifndef EXCHANGERATEGRAPH_HPP
#define EXCHANGERATEGRAPH_HPP

#include <vector>
#include <map>
#include <list>

#include "CurrencyOnExchange.hpp"
#include "RateAndTimestamp.hpp"
#include "ForwardAndBackwardRates.hpp"

namespace BER {

  typedef std::vector < std::vector < RateAndTimestamp > > Matrix;
  typedef std::vector < CurrencyOnExchange > IndexLookupTable;

  class ExchangeRateGraph {

    public:

      // Default constructor
      ExchangeRateGraph(); 

      // Check if the new exchange/currency is unknown, and update the graph structure accordingly
      bool updateGraphStructureIfNeeded(const CurrencyOnExchange & iCurrencyOnExchange);

      // If the timestamp is more recent, update the graph with a new echange rate
      bool updateRateAndTimestampIfNeeded(const CurrencyOnExchange & iFromCurrencyOnExchange,
                                          const CurrencyOnExchange & iToCurrencyOnExchange,
                                          const Rate & iForwardRate,
                                          const Rate & iBackwardRate,
                                          const Timestamp & iTimestamp);

      // Update both the graph structure and rates
      bool updateRates(const CurrencyOnExchange & iFromCurrencyOnExchange,
                       const CurrencyOnExchange & iToCurrencyOnExchange,
                       const Rate & iForwardRate,
                       const Rate & iBackwardRate,
                       const Timestamp & iTimestamp);

      // Find the best exchange rate following the Floyd-Warshall algorithm
      bool findBestExchangeRate(const CurrencyOnExchange & iFromCurrencyOnExchange,
                                const CurrencyOnExchange & iToCurrencyOnExchange,
                                Rate & oBestExchangeRate,
                                std::list< CurrencyOnExchange > & oPathForBestExchangeRate,
                                bool & oInfiniteCycle);
    
      // Calculate the rate given a path
      bool calculateRateFromPath(std::list< CurrencyOnExchange > & iPath,
                                 Rate & oRate);

      // Display the graph
      void display() const;

    private:

      // Return the number of vertexes of the graph
      int getCurrentNumberOfCurrenciesOnExchanges() const;

      // The graph, represented by weight matrix
      Matrix _graph;

      // A table to store the link between indexes in the matrix line/columns, and exchange/currency names
      IndexLookupTable _lookup;
  };
}

#endif
