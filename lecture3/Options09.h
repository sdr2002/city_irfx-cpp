//
// Created by sdr2002 on 29/10/24.
//

#ifndef OPTIONS08_H
#define OPTIONS08_H
#include "BinLattice02.h"
#include "../lecture2/BinModel02.h"

class Option {
    int N;
    public:
        void SetN(int N_){N=N_;}
        int GetN(){return N;}
        virtual double Payoff(double z)=0;
};

class EurOption: public virtual Option {
    public:
        double PriceByCRR(BinModel Model);
};

class AmOption: public virtual Option {
    public:
        double PriceBySnell(
            BinModel Model,
            BinLattice<double>& PriceTree,
            BinLattice<bool>& StoppingTree
        );
};

class Call: public EurOption, public AmOption {
    double K;
    public:
        friend std::string to_string(const Call& x) { return "Call"; }
        void SetK(double K_){K=K_;}
        int GetInputGridParameters();
        int GetDefaultGridParameters();
        double Payoff(double z);
};

class Put: public EurOption, public AmOption {
    double K;
    public:
        friend std::string to_string(const Put& x) { return "PUT"; }
        void SetK(double K_){K=K_;}
        int GetInputGridParameters();
        int GetDefaultGridParameters();
        double Payoff(double z);
};

#endif //OPTIONS08_H
