#include "BinModel01Sptr.h"
#include "Options02Sptr.h"  // Updated to use the smart pointer version of Options02
#include <iostream>
#include <cmath>
#include <memory>  // For smart pointers
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
    cout << "European call option price = "
         << PriceByCRR(*S0, *U, *D, *R, *N, *K)  // Dereference smart pointers to pass the values
         << endl << endl;

    return 0;
}
