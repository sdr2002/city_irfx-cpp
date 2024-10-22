#pragma once
#include <memory>            // For smart pointers
#include <functional>        // For std::function

using namespace std;

#ifndef OPTIONS03_H
#define OPTIONS03_H

double EuropeanCallPayoff(double z, double K);

double EuropeanPutPayoff(double z, double K);

double DigitalCallPayoff(double z, double K);

double DoubleKnockOutPayoff(double z, double K1, double K2);

double CRR_pv(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrK,
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrR, unique_ptr<int>& PtrN,
    const function<double(double, double)> &Payoff
);

double CRR_pv(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrK1, unique_ptr<double>& PtrK2,
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrR, unique_ptr<int>& PtrN,
    const function<double(double, double, double)>& Payoff
);

double CRR_delta(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrK,
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrR, unique_ptr<int>& PtrN,
    unique_ptr<double>& epsilon, const function<double(double, double)> &Payoff
);

double CRR_delta(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrK1, unique_ptr<double>& PtrK2,
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrR, unique_ptr<int>& PtrN,
    unique_ptr<double>& epsilon, const function<double(double, double, double)> &Payoff
);

double CRR_vega(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrK,
    unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrT, unique_ptr<double>& PtrR, unique_ptr<int>& PtrN,
    double epsilon,
    const function<double(double, double)> &Payoff,
    const function<void(
        unique_ptr<double>&, unique_ptr<double>&, unique_ptr<double>&,
        unique_ptr<double>&, unique_ptr<double>&, unique_ptr<int>&
    )> &SetUDfromVolt
);

double NewtonSymb(int N, int i);

double Analytic_pv(
    unique_ptr<double>& S0, unique_ptr<double>& U, unique_ptr<double>& D, unique_ptr<double>& R,
    unique_ptr<int>& N, unique_ptr<double>& K,
    const function<double(double, double)>& Payoff
);

double Analytic_pv(
    unique_ptr<double>& S0, unique_ptr<double>& U, unique_ptr<double>& D, unique_ptr<double>& R,
    unique_ptr<int>& N, unique_ptr<double>& K1, unique_ptr<double>& K2,
    const function<double(double, double, double)>& Payoff
);

double BS_EurCallOption_pv(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrR,
    unique_ptr<double>& PtrT, unique_ptr<double>& PtrK
);

double BS_EurPutOption_pv(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrR,
    unique_ptr<double>& PtrT, unique_ptr<double>& PtrK
);

#endif //OPTIONS03_H
