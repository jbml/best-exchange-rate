# Best exchange rate calculator 

The repository structrure is as follows:

* src/ folder: contains the source code and Makefile
* test/ folder: contains the test cases

---

## How to build the calculator

This tool has been tested with GCC 4.3.2

1. Go to the src/ folder
2. Compile the code with "make"

The binary will be located in the src/bin/ folder.

---

## How to use the calculator

The calculator gets it inputs from stdin, and will output its result on stdout.

2 different types of input are supported:

* Price update requests
* Best exchange rate requests

---

## Functional description

### Price update requests

#### Format

Input format: 

    <timestamp> <exchange> <source_currency> <destination_currency> <forward_factor> <backward_factor>

Input example:

    2017-11-01T09:42:23+00:00 KRAKEN BTC USD 1000.0 0.0009

Output format: 

    N/A (no output returned)

#### Validity check

The format of price update requests is checked, and any error will cause the price update to be **silently** ignored, meaning that there will be no error return on stdout to let the end user about the rejection.
The validity checks are as follows:

* The request has 6 tokens exactly, separated by one or more whitespaces
* The timestamp format is valid
* The forward and backward rates are valid and none of them is 0
* A multiplication of forward and backward rate is strictly less than 1

---

### Best exchange rate requests

#### Format

Input format:

    EXCHANGE_RATE_REQUEST <source_exchange> <source_currency> <destination_exchange> <destination_currency>

Input example:

    EXCHANGE_RATE_REQUEST GDAX BTC GDAX USD

Output format: 

    BEST_RATES_BEGIN <source_exchange> <source_currency> <destination_exchange> <destination_currency> <rate>
    <source_exchange, source_currency>
    <exchange, currency>
    <exchange, currency>
    ...
    <destination_exchange, destination_currency>
    BEST_RATES_END

Output format example:

    BEST_RATES_BEGIN GDAX USD GDAX BTC 0.00095
    GDAX, USD
    GDAX, BTC
    BEST_RATES_END

#### Validity checks

The format of the best rate request update is checked, and if any error if found, the request will be **silently** ignored by the tool, meaning that no error will be returned in stdout.
The validity checks are the following:

* The request has 5 tokens exactly, separated by one or more whitespaces
* First token is EXCHANGE_RATE_REQUEST exactly

#### Special cases

There are 2 special cases to consider for best exchange rate requests:

1. When it's impossible to build a series of trades to move from the source to the destination exchange/currency
2. When there is an infinite cycle in the graph such that the exchange rate can be arbitrarily high

#### Special case 1: no path from source to destination exchange/currency

In this case, the best exchange output will have the following format (note the **IMPOSSIBLE** keyword):

    BEST_RATES_BEGIN <source_exchange> <source_currency> <destination_exchange> <destination_currency> IMPOSSIBLE
    BEST_RATES_END

For example:

    BEST_RATES_BEGIN GDAX BTC GDAX XRP IMPOSSIBLE
    BEST_RATES_END

#### Special case 2: cycle in the graph

When there is an infitite cycle in the graph, it's not possible to determine what is the best rate because the best rate is infinite, assuming an infinite number of trades.
Assuming this is a *temporary* inconsistency between exchanges, the tool will rely on previous best rate calculations to provide an output in that case. This means that the list of trades to execute will be the same as the one return for the **latest** successful similar request. Note that in this case, the rate returned is not necessarily the same as the one returned for the latest successful calculation - the rate will be calcualated based on the most recent exchange rates received by the tool. This means that the rate returned is not necessarily the best one at this time.
If there was no similar successful request beforehand, the output will contain a special error (see below).

In case the tool returns a rate based on a previously calculated trade path, the output format is as follows (note the **(CACHED)** keyword):

    BEST_RATES_BEGIN <source_exchange> <source_currency> <destination_exchange> <destination_currency> <rate>
    <source_exchange, source_currency>
    <exchange, currency>
    <exchange, currency>
    ...
    <destination_exchange, destination_currency>
    BEST_RATES_END (CACHED)

For example:

    BEST_RATES_BEGIN GDAX BTC GDAX USD 90
    GDAX, BTC
    KRAKEN, BTC
    KRAKEN, USD
    GDAX, USD
    BEST_RATES_END (CACHED)

In case there was no similar request in the past, the tool will return an error with the following format (note the **CYCLE** keyword):

    BEST_RATES_BEGIN <source_exchange> <source_currency> <destination_exchange> <destination_currency> CYCLE
    BEST_RATES_END

For example:

    BEST_RATES_BEGIN GDAX BTC GDAX XRP CYCLE
    BEST_RATES_END
