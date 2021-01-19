#include <list>
#include <iostream>
#include <iomanip> 
#include <algorithm>
 
#include "ExchangeRateGraph.hpp"

namespace BER {

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  ExchangeRateGraph::ExchangeRateGraph() {
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  int ExchangeRateGraph::getCurrentNumberOfCurrenciesOnExchanges() const {
    return _lookup.size();
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  void ExchangeRateGraph::display() const {
    
    // Display the matrix
    std::cout << "==================================" << std::endl;
    std::cout << "-- Matrix display begin (size: " << _graph.size() << ") --" << std::endl;
    for(Matrix::const_iterator itLine = _graph.begin();
        itLine != _graph.end();
        ++itLine) {
      for(std::vector < RateAndTimestamp >::const_iterator itColumn = itLine->begin();
          itColumn != itLine->end();
          ++itColumn) {
        std::cout << "(" << std::setprecision(5) << std::setw(5) << itColumn->getRate() << ")(" << std::setw(5) << itColumn->getTimestamp() << ")";
        if(itColumn != itLine->end() - 1) {
          std::cout << " | ";
        }
      }
      std::cout << std::endl;
    }
  
    std::cout << std::endl;

    // Display the lookup
    std::cout << "-- Lookup table display begin (size: " << getCurrentNumberOfCurrenciesOnExchanges() << ") --" << std::endl;
    for(int itLookup = 0;
        itLookup < getCurrentNumberOfCurrenciesOnExchanges();
        ++itLookup) {
      std::cout << itLookup << " --> (" << _lookup[itLookup].getCurrencyCode() << "," 
                                        << _lookup[itLookup].getExchangeCode() << ")" << std::endl;
    }
    std::cout << "==================================" << std::endl;
  } 

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  bool ExchangeRateGraph::updateGraphStructureIfNeeded(const CurrencyOnExchange & iCurrencyOnExchange) {

    // Scan the lookup table for several reasons
    // 1. Check if this currency on this exchange is already known
    // 2. Determine if there are other exchanges offering the same currency (this is needed if this currency is new on this exchange)
    bool isCurrencyOnExchangeAlreadyKnown = false;
    std::list<int> aListOfIndexesWithSameCurrency;

    for(int itLookup = 0;
        itLookup != _lookup.size();
        ++itLookup) {
        
      // When we find an exchange with the same currency code
      if(_lookup[itLookup].getCurrencyCode() == iCurrencyOnExchange.getCurrencyCode()) {

        // If this item in the lookup table also has the same exchange code, we know that we don't have to update the matrix structure
        if(_lookup[itLookup].getExchangeCode() == iCurrencyOnExchange.getExchangeCode()) {
          isCurrencyOnExchangeAlreadyKnown = true;
        }
        // Otherwise we still need to store this index in case the matrix structure needs to be updated
        // This is to store the exchange rate of 1 for same currencies across exchanges
        else {
          aListOfIndexesWithSameCurrency.push_back(itLookup);
        }
      }
    }

    // In case we don't find this currency and exchange pair in the map, it means we do not know about it yet
    // This means that we will have to insert it in the graph as a new matrix row/column, as well as insert in the lookup table
    if(isCurrencyOnExchangeAlreadyKnown == false) {

      // Insert in the lookup table
      _lookup.push_back(iCurrencyOnExchange);
      // Also use the new index in the list of index with same currencies (to have 1s in the matrix diagonal)
      aListOfIndexesWithSameCurrency.push_back(_lookup.size() - 1);

      // Insert in the graph
      // Add a new column by inserting one element at the end of each line
      // For example, for an 3x3 initial matrix, we add a new 4th column with 0s 
      // X X X           X X X 0
      // X X X  becomes  X X X 0
      // X X X           X X X 0
      RateAndTimestamp aZeroRate(0,0);
      for(Matrix::iterator itLine = _graph.begin();
          itLine != _graph.end();
          ++itLine) {
        itLine->push_back(aZeroRate);
      }

      // Add a new line to the matrix, at the bottom, with the right size pre-allocated
      // For example, following the previous example, we end up with a 4x4 matrix as follows
      // X X X 0            X X X 0
      // X X X 0  becomes   X X X 0 
      // X X X 0            X X X 0
      //                    0 0 0 0 
      std::vector<RateAndTimestamp> aNewLineForTheMatrix(_lookup.size());
      for(int itNewLine = 0;
          itNewLine != _lookup.size();
          ++itNewLine) {
        aNewLineForTheMatrix[itNewLine] = aZeroRate;
      }
      _graph.push_back(aNewLineForTheMatrix);

      // Set the rates to 1 for similar currencies on other exchanges
      RateAndTimestamp aSameCurrencyRateAndTimestamp(1,0);
      for(std::list<int>::const_iterator itIndexOfSameCurrency = aListOfIndexesWithSameCurrency.begin();
          itIndexOfSameCurrency != aListOfIndexesWithSameCurrency.end();
          ++itIndexOfSameCurrency) {
        _graph[_lookup.size() - 1][*itIndexOfSameCurrency] = aSameCurrencyRateAndTimestamp;
        _graph[*itIndexOfSameCurrency][_lookup.size() - 1] = aSameCurrencyRateAndTimestamp;
      }    

    }
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  bool ExchangeRateGraph::updateRateAndTimestampIfNeeded(const CurrencyOnExchange & iFromCurrencyOnExchange,
                                                         const CurrencyOnExchange & iToCurrencyOnExchange,
                                                         const Rate & iForwardRate,
                                                         const Rate & iBackwardRate,
                                                         const Timestamp & iTimestamp) {

    // Find FROM and TO indexes
    int aFromIndex = -1;
    int aToIndex = -1;    

    for(int itLookup = 0;
        itLookup != _lookup.size();
        ++itLookup) {

      if(_lookup[itLookup].getCurrencyCode() == iFromCurrencyOnExchange.getCurrencyCode() &&
         _lookup[itLookup].getExchangeCode() == iFromCurrencyOnExchange.getExchangeCode()) {
        aFromIndex = itLookup;
      }

      if(_lookup[itLookup].getCurrencyCode() == iToCurrencyOnExchange.getCurrencyCode() &&
         _lookup[itLookup].getExchangeCode() == iToCurrencyOnExchange.getExchangeCode()) {
        aToIndex = itLookup;
      }
    }

    // We should always find the currency/exchange in the graph
    if(aFromIndex == -1 || aToIndex == -1) {
      return false;
    }
   
    // Check that the new timestamp is more recent than the existing one, otherwise there is nothing to do
    Timestamp anExistingTimestamp = _graph[aFromIndex][aToIndex].getTimestamp();
    if(anExistingTimestamp >= iTimestamp) {
      return true;
    } 

    // Update the rates and timestamps
    RateAndTimestamp aForwardRateAndTimestamp(iForwardRate, iTimestamp); 
    _graph[aFromIndex][aToIndex] = aForwardRateAndTimestamp;
    RateAndTimestamp aBackwardRateAndTimestamp(iBackwardRate, iTimestamp);
    _graph[aToIndex][aFromIndex] = aBackwardRateAndTimestamp;

    return true;
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  bool ExchangeRateGraph::updateRates(const CurrencyOnExchange & iFromCurrencyOnExchange,
                                      const CurrencyOnExchange & iToCurrencyOnExchange,
                                      const Rate & iForwardRate,
                                      const Rate & iBackwardRate,
                                      const Timestamp & iTimestamp) {

    // Update the graph with new nodes corresponding to the new exchange data, in case we did not know about it 
    updateGraphStructureIfNeeded(iFromCurrencyOnExchange);
    updateGraphStructureIfNeeded(iToCurrencyOnExchange);

    // Update the rate and timestamp, depending on the previous timestamp
    updateRateAndTimestampIfNeeded(iFromCurrencyOnExchange,
                                   iToCurrencyOnExchange,
                                   iForwardRate, 
                                   iBackwardRate,
                                   iTimestamp);
  }

  // ///////////////////////////////////////////////////////////////////////////////////////////////////
  bool ExchangeRateGraph::findBestExchangeRate(const CurrencyOnExchange & iFromCurrencyOnExchange,
                                               const CurrencyOnExchange & iToCurrencyOnExchange,
                                               Rate & oBestExchangeRate,
                                               std::list< CurrencyOnExchange > & oPathForBestExchangeRate,
                                               bool & oInfiniteCycle) {

    // Make a copy of the matrix where we can apply the Floyd-Warshall algorithm
    Matrix aCopyOfTheGraph = _graph; 
    int aNumberOfVertex = getCurrentNumberOfCurrenciesOnExchanges();

    // Get the FROM and TO indexes
    int aFromIndex = -1;
    int aToIndex = -1;

    for(int itLookup = 0;
        itLookup != _lookup.size();
        ++itLookup) {

      if(_lookup[itLookup].getCurrencyCode() == iFromCurrencyOnExchange.getCurrencyCode() &&
         _lookup[itLookup].getExchangeCode() == iFromCurrencyOnExchange.getExchangeCode()) {
        aFromIndex = itLookup;
      }

      if(_lookup[itLookup].getCurrencyCode() == iToCurrencyOnExchange.getCurrencyCode() &&
         _lookup[itLookup].getExchangeCode() == iToCurrencyOnExchange.getExchangeCode()) {
        aToIndex = itLookup;
      }
    }

    if(aFromIndex == -1 || aToIndex == -1) {
      return false;
    }
   
    // Create a second matrix for the paths
    std::vector< std::vector <int> > aPathMatrix(aNumberOfVertex);
    for(int i = 0; i < aNumberOfVertex; ++i) {

      // Allocate a line
      std::vector <int> aPathMatrixLine(aNumberOfVertex);

      for(int j = 0; j < aNumberOfVertex; ++j) {

        if(aCopyOfTheGraph[i][j].getRate() != 0) {
          aPathMatrixLine[j] = j;
        }
        else { 
          aPathMatrixLine[j] = -1;
        }
      }
      aPathMatrix[i] = aPathMatrixLine;
    }

    // Apply the Floyd-Warshall algorithm
    for(int k = 0; k < aNumberOfVertex; ++k) {
      for(int i = 0; i < aNumberOfVertex; ++i) {
        for(int j = 0; j < aNumberOfVertex; ++j) {

          // When we find a new path
          if(aCopyOfTheGraph[i][j].getRate() < aCopyOfTheGraph[i][k].getRate() * aCopyOfTheGraph[k][j].getRate()) {
            RateAndTimestamp anUpdatedRate(aCopyOfTheGraph[i][k].getRate() * aCopyOfTheGraph[k][j].getRate(), 0);   
            aCopyOfTheGraph[i][j] = anUpdatedRate;
            aPathMatrix[i][j] = aPathMatrix[i][k];
          }
        }
      }
    }

    // Display best rate matrix
    //std::cout << "Display best rate matrix" << std::endl;
    for(int i = 0; i < aNumberOfVertex; ++i) {
      for(int j = 0; j < aNumberOfVertex; ++j) {
        //std::cout << aCopyOfTheGraph[i][j].getRate() << " | ";
      }
      //std::cout << std::endl;
    }

    // Check if there is any infinite cycle
    bool anInfiniteCycleDetected = false;
    for(int i = 0; i < aNumberOfVertex; ++i) {
      if(aCopyOfTheGraph[i][i].getRate() > 1) {
        anInfiniteCycleDetected = true;
        break;
      }
    }
     
    //std::cout << "Display final path matrix" << std::endl;
    for(int i = 0; i < aNumberOfVertex; ++i) {
      for(int j = 0; j < aNumberOfVertex; ++j) {
        //std::cout << aPathMatrix[i][j] << " ";
      }
      //std::cout << std::endl;
    }

    // Determine the best exchange rate
    if(anInfiniteCycleDetected == true) {
      oInfiniteCycle = true;
      oBestExchangeRate = -1;
    }
    else {
      oBestExchangeRate = aCopyOfTheGraph[aFromIndex][aToIndex].getRate();
    }

    // Determine the best path
    std::list<int> aListOfAlreadyVisitedNodes;
    if(aPathMatrix[aFromIndex][aToIndex] != -1) {

      oPathForBestExchangeRate.push_back(iFromCurrencyOnExchange);
      
      while(aFromIndex != aToIndex) {
        aFromIndex = aPathMatrix[aFromIndex][aToIndex];
        oPathForBestExchangeRate.push_back(_lookup[aFromIndex]);
     
        std::list<int>::iterator itSearch = std::find(aListOfAlreadyVisitedNodes.begin(),
                                                      aListOfAlreadyVisitedNodes.end(), 
                                                      aFromIndex);
        // If we did not have this element already found, we can continue
        if(itSearch == aListOfAlreadyVisitedNodes.end()) {
          aListOfAlreadyVisitedNodes.push_back(aFromIndex);
        }
        // Oterwise we need to stop here as we are in an infinite cycle
        else {
          break;
        }
      }
    } 

    return true;
  }
   
  // ///////////////////////////////////////////////////////////////////////////////////
  bool ExchangeRateGraph::calculateRateFromPath(std::list< CurrencyOnExchange > & iPath,
                                                Rate & oRate) {

    // If the path is empty -> unexpected error case
    if(iPath.size() == 0) {
      oRate = 0;
      return false;
    }
 
    // If the path size is 1 only, it means that we are looking at the same FROM and TO exchange/currency - rate is 1
    if(iPath.size() == 1) {
      oRate = 1;
      return true;
    } 
  
    std::list<int> anIndexPath;
    // Convert the CurrencyOnExchange path to index path
    for(std::list< CurrencyOnExchange >::const_iterator itPath = iPath.begin();
        itPath != iPath.end(); 
        ++itPath) {
  
      for(int itLookup = 0;
          itLookup != _lookup.size();
          ++itLookup) {

        if(_lookup[itLookup] == *itPath) {
          anIndexPath.push_back(itLookup);
        }
      }
    }

    // Follow the rates matrix to calculate the current rate
    oRate = 1;
    // Create an iterator at the 1st and 2nd positions
    std::list<int>::const_iterator itIndexBackward = anIndexPath.begin();
    std::list<int>::const_iterator itIndexForward = anIndexPath.begin();
    itIndexForward++;

    for(;
        itIndexForward != anIndexPath.end();
        ++itIndexForward, ++itIndexBackward) {

      oRate = oRate * _graph[*itIndexBackward][*itIndexForward].getRate();
    }  
  }
}
