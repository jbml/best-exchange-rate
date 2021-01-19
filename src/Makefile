ExchangeRateCalculator : ExchangeRateCalculator.o InputRequest.o ExchangeRateGraph.o CurrencyOnExchange.o ForwardAndBackwardRates.o RateAndTimestamp.o InputRequestFactory.o PriceUpdateInput.o ExchangeRateRequest.o
	mkdir -p bin
	g++ -g -std=c++0x -o bin/ExchangeRateCalculator obj/ExchangeRateCalculator.o obj/InputRequest.o obj/ExchangeRateGraph.o obj/CurrencyOnExchange.o obj/ForwardAndBackwardRates.o obj/RateAndTimestamp.o obj/InputRequestFactory.o obj/PriceUpdateInput.o obj/ExchangeRateRequest.o -Lobj 

ExchangeRateCalculator.o : ExchangeRateCalculator.cpp 
	mkdir -p obj
	g++ -g -std=c++0x -o obj/ExchangeRateCalculator.o -c -I. ExchangeRateCalculator.cpp 

InputRequest.o : InputRequest.cpp InputRequest.hpp
	mkdir -p obj
	g++ -g -std=c++0x -o obj/InputRequest.o -c -I. InputRequest.cpp 

InputRequestFactory.o : InputRequestFactory.cpp InputRequestFactory.hpp
	mkdir -p obj
	g++ -g -std=c++0x -o obj/InputRequestFactory.o -c -I. InputRequestFactory.cpp

PriceUpdateInput.o : PriceUpdateInput.cpp PriceUpdateInput.hpp
	mkdir -p obj
	g++ -g -std=c++0x -o obj/PriceUpdateInput.o -c -I. PriceUpdateInput.cpp

ExchangeRateRequest.o : ExchangeRateRequest.cpp ExchangeRateRequest.hpp
	mkdir -p obj
	g++ -g -std=c++0x -o obj/ExchangeRateRequest.o -c -I. ExchangeRateRequest.cpp

ExchangeRateGraph.o : ExchangeRateGraph.cpp ExchangeRateGraph.hpp
	mkdir -p obj
	g++ -g -std=c++0x -o obj/ExchangeRateGraph.o -c -I. ExchangeRateGraph.cpp

CurrencyOnExchange.o : CurrencyOnExchange.cpp CurrencyOnExchange.hpp
	mkdir -p obj
	g++ -g -std=c++0x -o obj/CurrencyOnExchange.o -c -I. CurrencyOnExchange.cpp

ForwardAndBackwardRates.o : ForwardAndBackwardRates.cpp ForwardAndBackwardRates.hpp
	mkdir -p obj
	g++ -std=c++0x -g -o obj/ForwardAndBackwardRates.o -c -I. ForwardAndBackwardRates.cpp

RateAndTimestamp.o : RateAndTimestamp.cpp RateAndTimestamp.hpp
	mkdir -p obj
	g++ -std=c++0x -g -o obj/RateAndTimestamp.o -c -I. RateAndTimestamp.cpp

clean :
	rm -f bin/ExchangeRateCalculator obj/ExchangeRateCalculator.o obj/InputRequest.o obj/ExchangeRateGraph.o obj/CurrencyOnExchange.o obj/ForwardAndBackwardRates.o obj/RateAndTimestamp.o obj/InputRequestFactory.o obj/InputRequest.o obj/PriceUpdateInput.o obj/ExchangeRateRequest.o
