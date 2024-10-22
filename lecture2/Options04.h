#ifndef Options04_h
#define Options04_h

#include "BinModel02.h"

// Inputting and displaying option data
int GetInputGridParameters(int& N, double& K);

// Pricing European option
double PriceByCRR(BinModel Model, int N, double K, double (*Payoff)(double z, double K));

// Computing call payoff
double CallPayoff(double z, double K);

// Computing put payoff
double PutPayoff(double z, double K);

#endif
