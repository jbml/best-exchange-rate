#include <iostream>

#include "ExchangeRateGraph.hpp"
#include "InputRequestFactory.hpp"
#include "InputRequest.hpp"

int main (int argc, char *argv[]) {

  // Create a graph and factory to generate input requests
  BER::ExchangeRateGraph myGraph;
  BER::InputRequestFactory aFactory;
  std::string anInputRequestRawLine;

  // Loop on stdin requests
  while (std::getline(std::cin, anInputRequestRawLine)) {

    // Attempt to create a request
    BER::InputRequest * anInputRequest = aFactory.buildInputRequest(anInputRequestRawLine);
    
    // Silently ignore invalid requests, and process valid requests
    if(anInputRequest != NULL) {
      // Associate the graph to the request
      anInputRequest->createLinkWithGraph(&myGraph);
      // Execute the request action, depending on the request type
      anInputRequest->execute();
    }

    aFactory.clean();
  }
}
