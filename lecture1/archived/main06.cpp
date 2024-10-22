#include "BinModel01.h"
#include "Options02.h"
#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    double S0, U, D, R;

    if (GetInputGridParameters(S0, U, D, R) == 1) return 1;

    double K;  // strike price
    int N;     // steps to expiry

    cout << "Enter call option data:" << endl;
    GetInputGridParameters(&N, &K);
    cout << "European call option price = "
         << PriceByCRR(S0, U, D, R, N, K)
         << endl << endl;

    return 0;
}
