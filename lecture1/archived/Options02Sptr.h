#ifndef OPTIONS02_SPTR_H
#define OPTIONS02_SPTR_H

#include <memory>  // For smart pointers

// Function to get input data using smart pointers
int GetInputGridParameters(std::unique_ptr<int>& PtrN, std::unique_ptr<double>& PtrK);

// Function to price the European call option using the Cox-Ross-Rubinstein model
double PriceByCRR(double S0, double U, double D, double R, int N, double K);

// Function to compute the payoff of a call option
double CallPayoff(double z, double K);

#endif
