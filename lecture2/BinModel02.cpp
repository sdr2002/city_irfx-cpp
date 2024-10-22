#include "BinModel02.h"
#include <iostream>
#include <cmath>
using namespace std;

// Constructor: Initialize member variables to zero
BinModel::BinModel() : S0(0), U(0), D(0), R(0) {}

// Shared method to check parameters
int BinModel::checkData()
{
    // Make sure that 0 < S0, -1 < D < U, -1 < R
    if (S0 <= 0 || U <= -1.0 || D <= -1.0 || R <= -1.0 || U <= D)
    {
        cout << "Illegal data ranges" << endl;
        cout << "Terminating program" << endl;
        return 1;
    }

    // Check for arbitrage: R should be between U and D
    if (R >= U || R <= D)
    {
        cout << "Arbitrage exists" << endl;
        cout << "Terminating program" << endl;
        return 1;
    }

    cout << "Input data checked" << endl;
    cout << "There is no arbitrage" << endl;
    return 0;
}

// Method to compute the risk-neutral probability
double BinModel::RiskNeutProb()
{
    return (R - D) / (U - D);
}

// Method to compute stock price at node (n, i)
double BinModel::S(int n, int i)
{
    return S0 * pow(1 + U, i) * pow(1 + D, n - i);
}

// Method to get input data from the user
int BinModel::GetInputGridParameters()
{
    // Entering data
    cout << "Enter S0: "; cin >> S0;
    cout << "Enter U: "; cin >> U;
    cout << "Enter D: "; cin >> D;
    cout << "Enter R: "; cin >> R;
    cout << endl;

    // Call shared checker function
    return checkData();
}

// Method to set default values
int BinModel::GetDefaultGridParameters()
{
    S0 = 100;  // Initial stock price
    U = 0.1;  // Up factor
    D = -0.1; // Down factor
    R = 0.01;     // Risk-free rate

    cout << "Default data set:" << endl;
    cout << "S0 = " << S0 << ", U = " << U << ", D = " << D << ", R = " << R << endl;

    // Call shared checker function
    return checkData();
}

// Getter for R (risk-free rate)
double BinModel::GetR()
{
    return R;
}

double BinModel::GetS0() {
    return S0;
}

void BinModel::SetS0(const double S0new) {
    S0 = S0new;
}
