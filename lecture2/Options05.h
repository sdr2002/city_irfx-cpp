#ifndef Options05_h
#define Options05_h

#include "BinModel02.h"

class EurOption
{
private:
    int N;  // steps to expiry
    double (*Payoff)(double z, double K);  // pointer to payoff function

public:
    void SetN(int N_) { N = N_; }
    void SetPayoff(double (*Payoff_)(double z, double K)) { Payoff = Payoff_; }
    double PriceByCRR(BinModel Model, double K);
};

// Computing call payoff
double EurCallPayoff(double z, double K);

// Computing put payoff
double EurPutPayoff(double z, double K);

class EurCall : public EurOption
{
private:
    double K;  // strike price

public:
    EurCall() { SetPayoff(EurCallPayoff); }
    double GetK() { return K; }
    int GetInputGridParameters();
};

// Computing put payoff
class EurPut : public EurOption
{
private:
    double K;  // strike price

public:
    EurPut() { SetPayoff(EurPutPayoff); }
    double GetK() { return K; }
    int GetInputGridParameters();
};

#endif
