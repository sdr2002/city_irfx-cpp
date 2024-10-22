#include "BinModel01Sptr.h"
#include "Options03.h"  // Updated to use the smart pointer version of Options03
#include <iostream>
#include <cmath>
#include <memory>       // For smart pointers
#include <functional>   // For function
using namespace std;

int main()
{
    unique_ptr<double> S0, U, D, R;

    if (GetInputGridParameters(S0, U, D, R) == 1) return 1;

    // Using smart pointers for strike price (K) and steps to expiry (N)
    unique_ptr<int> N;     // steps to expiry
    unique_ptr<double> K;  // strike price

    cout << "Enter call option data:" << endl;
    GetInputGridParameters(N, K);  // Modified to use smart pointers

    // Accessing the values pointed by the smart pointers for pricing
    function p = CallPayoff;
    cout << "CRR European call option price = "
         << PriceByCRR(S0, U, D, R, N, K, p)  // Pass CallPayoff using function
         << endl << endl;

    p = PutPayoff;
    cout << "CRR European put option price = "
         << PriceByCRR(S0, U, D, R, N, K, p)  // Pass PutPayoff using function
         << endl << endl;

    cout << "Analytic European call option price = " << endl
         << PriceAnalytic(S0, U, D, R, N, K, CallPayoff) << endl << endl;

    cout << "BS volatility = "
         << VolatilityBsFromUD(U, D, R)
         << endl << endl;

    return 0;
}