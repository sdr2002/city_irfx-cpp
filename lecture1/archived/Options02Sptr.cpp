#include "Options02.h"
#include "BinModel01.h"  // Commented out <iostream>, <cmath>, std
#include <memory>        // For smart pointers
#include <iostream>
#include <cmath>
using namespace std;

int GetInputGridParameters(unique_ptr<int>& PtrN, unique_ptr<double>& PtrK)
{
    int tempN;
    double tempK;

    cout << "Enter steps to expiry N: "; cin >> tempN;
    cout << "Enter strike price K: "; cin >> tempK;
    cout << endl;

    // Assigning input values to smart pointers
    PtrN = make_unique<int>(tempN);
    PtrK = make_unique<double>(tempK);

    return 0;
}

double PriceByCRR(double S0, double U, double D, double R, int N, double K)
{
    double q = riskNeutralProb(U, D, R);

    // Use unique_ptr for the Price array
    unique_ptr<double[]> Price = make_unique<double[]>(N+1);

    for (int i = 0; i <= N; i++)
    {
        Price[i] = CallPayoff(S(S0, U, D, N, i), K);
    }

    for (int n = N - 1; n >= 0; n--)
    {
        for (int i = 0; i <= n; i++)
        {
            Price[i] = (q * (Price[i+1]) + (1 - q) * (Price[i])) / (1 + R);
        }
    }

    return Price[0];  // Return the first price
}

double CallPayoff(double z, double K)
{
    if (z > K) return z - K;
    return 0.0;
}
