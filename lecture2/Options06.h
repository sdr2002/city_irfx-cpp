#ifndef Options06_h
#define Options06_h

#include <stdexcept>

#include "BinModel02.h"

class EurOption {
private:
    int N; // steps to expiry
public:
    void SetN(int N_){ N = N_; }

    // Payoff defined to return 0.0 for pedagogical purposes.
    // To use a pure virtual function, replace with virtual double Payoff(double z)=0;
    virtual double Payoff(double z) { return 0.0; }

    // Pricing European option
    double PriceByCRR(BinModel Model);
    double DeltaByCRR(BinModel Model, double epsilon);

    double PriceByAnalyticBinomial(BinModel Model);

    virtual double PriceByBlackScholes(BinModel Model) {throw std::invalid_argument("Not implemented");}
};

class EurCall: public EurOption {
private:
    double K; // strike price
public:
    void SetK(double K_){ K = K_; }
    int GetInputGridParameters();
    int GetDefaultGridParameters();

    virtual double Payoff(double z);
};

class EurPut: public EurOption {
private:
    double K; // strike price
public:
    void SetK(double K_){ K = K_; }
    int GetInputGridParameters();
    int GetDefaultGridParameters();

    virtual double Payoff(double z);
};

class EurDoubleKnockOut: public EurOption {
private:
    double Kl; // strike price
    double Kh; // strike price
public:
    void SetK(double Kl_){ Kl = Kl_; }
    void SetK2(double Kh_){ Kh = Kh_; }
    int GetInputGridParameters();
    int GetDefaultGridParameters();

    virtual double Payoff(double z);
};

#endif
