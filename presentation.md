# Counting Money
## in C++

##### Maciej Gajewski

====

#### About me

* Maciek Gajewski [maciej.gajewski0@gmail.com](mailto:maciej.gajewski0@gmail.com)

<img src="img/maciek.jpg" height="200"/>

Note:

* 30 years of programming, 20 years of C++
* 2000-2010 Wrocław (Pruftechnik, Tieto)
* 2010-2018 London, Amsterdam, 
* HFT, teaching (Tibra, Optiver)

====
<!-- .slide: data-background-image="img/KEN-WEBSITE-HEADER-uai-2064x1161.jpg" -->

<img src="img/Kenetic-Logo-White-1.svg" height="100"/>

Notes:

* Currently working for Kenetic, a HK-based company trading cryptocurrencies.
* End-to-end trading system
* Majority of the code in Python, but the fats bits in C++
* This presentation is about data types used in the C++ to store financial values
* I'm going to show excerpts from actual production code

====
<!-- .slide: data-background-image="img/huobi-btc-usdt_2020-04-25_16-03-52.png" -->
<!-- .slide:data-background-size="contain" -->


Notes:

* What values do we need to store?
 - price depth, trades, postions
* On this picture: Hubi BTc/USDT, 2020-04-25 16:03
* Introducing price depth:
    - Simple price is not enough
    - At the core of every exchange as a device called 'matching engine'
    - Matching engine stores all active orders for an instrument (order book)
    - When new order is entered, it is matched with existing orders
    - Example - buying or selling 1 BTC
    - If there is no match, the order (possibly) joins the book
    - If the uis a match, trade happens
    - What we dee is 'price depth', a view of the oder book

https://www.binance.com/pl/trade/BTC_USDT
https://www.huobi.com/en-us/exchange/btc_usdt/


====

## Price depth

``` cpp
using Price = ???;
using Quantity = ???;

struct DepthLevel {
  Price mPrice;
  Quantity mQty;
};

using DepthLevels = std::vector<DepthLevel>;

struct Depth {
  DepthLevels mBids; // sorted by price, descending
  DepthLevels mAsks; // sorted by price, ascending 
};
```

====

## Requirements for the type

* Exact ==
* Comparison, <, <=, >, >=
* Arithmetics: +, -, +=, -=
* No loss of precision!
* conversion to/from string

Notes:

* The type must be fast. If don;t need speed, we use Python
* Depth diffs and multi-depth operations require operators to be fast!

====

## What are the options

1. floating point numbers
2. fixed point numbers
3. decimal floating point
4. arbitrary precision

Notes:

These are the options that come to mind when looking for a suitable type.

====

### Floating point types

float, double : IEEE 754

<img src="img/Single-Precision-vs-Double-Precision.png"/>

Notes:

* We have our good'ol floating point types built into the language and the CPU
Image source: https://www.thecrazyprogrammer.com/2018/04/single-precision-vs-double-precision.html

====

### Floating point types

Pros:

* Fast
* Available out of the box

Notes:

* The built-in floating point types are fast, they are handled by the CPU itself (on arch of interest, ie x86-64)
* They are readily available, with the entire complement of operations

====

### Floating point types

Cons

* Inexact equality comparison
* Silent loss of precision

_What Every Programmer Should Know About Floating-Point Arithmetic_
https://floating-point-gui.de/

Notes:

* There is a fantastic article called _What Every Programmer Should Know About Floating-Point Arithmetic_

====

### Floating point types

Bad at storing decimal numbers

``` cpp
0.1 + 0.2 = 0.30000000000000004
```

Notes:

* Example form the article
* Floating point numbers are famously bad at storing decimal fractions

====

### Floating point types

Inexact comparison

```cpp
double a = parse1("1234.56789");
double b = parse2("1234.56789");
assert(a == b);

std::map<double, T> map;
map[a] = data;
assert(map.find(b) != map.end());
```

Notes:

* The build-in comparison is bitwise
* The same value, after some operations, can slightly differ
* Even using different string parser, ie JSON vs database input, can yield different bits
* The solution: create a "compare with tolerance", but this is slow (and incorrect)
* Trying to use double as a key in a map is an invitation to a world of pain
* I've seen this done, working for years, and then suddenly not working in mature, production code,
* ... poor programmers looking for a for a fix whole night

====

## Arbitrary precision types

* Boost.Multiprecision
* GMP - https://gmplib.org/
* MPDEC (Python's Decimal) - https://www.bytereef.org/mpdecimal/

Notes:

* Arbitrary precision types have no rounding/precision issues
* Bat are universally slow
* We are confined by the limits of our CPU!

====

## Arbitrary precision types

``` c
#include <mpdecimal.h>

typedef struct mpd_t {
       uint8_t flags;       // [memory flags] | [specials] | sign
       mpd_ssize_t exp;     // exponent
       mpd_ssize_t digits;  // number of decimal digits
       mpd_ssize_t len;     // number of words in use
       mpd_ssize_t alloc;   // number of allocated words
       mpd_uint_t *data;    // words
} mpd_t;
```

Notes:

* This is data-type used internally by MPDEC
* One can see a variable-sized, heap allocated part.

## Decimal floating point types

--

``` cpp
#include <decimal/decimal>

using Price = std::decimal::decimal64;
using Quantity = std::decimal::decimal64;

auto findLevel(DepthLevels& levels, const Pr& p) {
    return std::find_if(levels.begin(), levels.end(), [&](auto& l) {
         return l.mPrice == p;  });
}

auto accumulate(const std::vector<Trade>& trades) {
    return std::accumulate(
        trades.begin(), trades.end(), Qt{0},
        []( const Qt& q, const Trade& t) {
            return t.mQty + q;
        }
    );
}
```

====

## TODO decimal floating point
https://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/a01578.html

====

The last slide:

* TODO: this presentation
* maciej.gajewski0@gmail.com
* https://floating-point-gui.de/