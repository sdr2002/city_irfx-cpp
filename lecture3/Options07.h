#ifndef Options07_h
#define Options07_h

#include "../lecture2/BinModel02.h"

class EurOption
{
private:
    int N; // steps to expiry
public:
    void SetN(int N_){ N = N_; }
    virtual double Payoff(double z) = 0;
    // Pricing European option
    double PriceByCRR(BinModel Model);
};

class AmOption
{
private:
    int N; // steps to expiry
public:
    void SetN(int N_){ N = N_; }
    virtual double Payoff(double z) = 0;
    // Pricing American option
    double PriceBySnell(BinModel Model);
};

class Call: public EurOption, public AmOption
{
private:
    double K; // strike price
public:
    void SetK(double K_){ K = K_; }
    int GetInputGridParameters();
    int GetDefaultGridParameters();
    double Payoff(double z);
};

class Put: public EurOption, public AmOption
{
private:
    double K; // strike price
public:
    void SetK(double K_){ K = K_; }
    int GetInputGridParameters();
    int GetDefaultGridParameters();
    double Payoff(double z);
};

#endif
