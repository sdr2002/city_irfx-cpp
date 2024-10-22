#include "BinModel01Sptr.h"  // Commented out <iostream>, <cmath>, std
#include <memory>            // For smart pointers
#include <iostream>
#include <cmath>
#include <functional>        // For std::function

using namespace std;

/**
 * @brief Payoff function for European Call Option.
 * @param z Current stock price.
 * @param K Strike price.
 * @return Payoff value.
 */
double EuropeanCallPayoff(double z, double K) {
    return max(z - K, 0.0);
}

double EuropeanPutPayoff(double z, double K) {
    return max(K - z, 0.0);
}

double DigitalCallPayoff(double z, double K)
{
    return z >= K;
}

double DoubleKnockOutPayoff(double z, double K1, double K2)
{
    if (K1 > K2) {
        const double intermediate = K1;
        K1 = K2;
        K2 = intermediate;
    }
    return (z >= K1) and (z <= K2);
}

/**
 * @brief Binomial CRR pricing model for European Call Option.
 * @param PtrS0 Initial stock price.
 * @param PtrK Strike price.
 * @param PtrU Up move factor.
 * @param PtrD Down move factor.
 * @param PtrR Risk-free rate.
 * @param PtrN Number of steps.
 * @param Payoff Payoff function (e.g., call or put option).
 * @return Option price.
 */
double CRR_pv(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrK,
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrR, unique_ptr<int>& PtrN,
    const function<double(double, double)> &Payoff
) {
    double q = riskNeutralProb(*PtrU, *PtrD, *PtrR);  // Risk-neutral probability
    vector<double> price(*PtrN + 1);

    // Calculate the payoff at maturity
    for (int i = 0; i <= *PtrN; ++i) {
        price[i] = Payoff(*PtrS0 * pow(1 + *PtrU, i) * pow(1 + *PtrD, *PtrN - i), *PtrK);
    }

    // Step backwards through the tree to calculate the option price
    for (int n = *PtrN - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            price[i] = (q * price[i + 1] + (1 - q) * price[i]) / (1 + *PtrR);
        }
    }

    return price[0];  // Return the first price (initial node)
}

double CRR_pv(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrK1, unique_ptr<double>& PtrK2,
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrR, unique_ptr<int>& PtrN,
    const function<double(double, double, double)>& Payoff
) {
    double q = riskNeutralProb(*PtrU, *PtrD, *PtrR);  // Risk-neutral probability
    vector<double> price(*PtrN + 1);

    // Calculate the payoff at maturity
    for (int i = 0; i <= *PtrN; ++i) {
        price[i] = Payoff(*PtrS0 * pow(1 + *PtrU, i) * pow(1 + *PtrD, *PtrN - i), *PtrK1, *PtrK2);
    }

    // Step backwards through the tree to calculate the option price
    for (int n = *PtrN - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            price[i] = (q * price[i + 1] + (1 - q) * price[i]) / (1 + *PtrR);
        }
    }

    return price[0];  // Return the first price (initial node)
}

/**
 * @brief Function to calculate the delta of an option using the CRR binomial model.
 * @param S0 Initial stock price.
 * @param K Strike price.
 * @param U Up move factor.
 * @param D Down move factor.
 * @param R Risk-free rate.
 * @param N Number of steps.
 * @param epsilon Small bump value for stock price.
 * @param Payoff Payoff function (e.g., call or put option).
 * @return Delta value.
 */
double CRR_delta(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrK,
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrR, unique_ptr<int>& PtrN,
    unique_ptr<double>& epsilon, const function<double(double, double)> &Payoff
) {
    auto S0pp = make_unique<double>(*PtrS0 * (1 + *epsilon));
    double PV_S0pp = CRR_pv(S0pp, PtrK, PtrU, PtrD, PtrR, PtrN, Payoff);  // Price with bumped S0

    auto S0mm = make_unique<double>(*PtrS0 * (1 - *epsilon));
    double PV_S0mm = CRR_pv(S0mm, PtrK, PtrU, PtrD, PtrR, PtrN, Payoff);  // Price with reduced S0

    return (PV_S0pp - PV_S0mm) / (*PtrS0 * 2 * *epsilon);  // Delta is the change in price divided by change in S0
}

double CRR_delta(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrK1, unique_ptr<double>& PtrK2,
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrR, unique_ptr<int>& PtrN,
    unique_ptr<double>& epsilon, const function<double(double, double, double)> &Payoff
) {
    auto S0pp = make_unique<double>(*PtrS0 * (1 + *epsilon));
    double PV_S0pp = CRR_pv(S0pp, PtrK1, PtrK2, PtrU, PtrD, PtrR, PtrN, Payoff);  // Price with bumped S0

    auto S0mm = make_unique<double>(*PtrS0 * (1 - *epsilon));
    double PV_S0mm = CRR_pv(S0mm, PtrK1, PtrK2, PtrU, PtrD, PtrR, PtrN, Payoff);  // Price with reduced S0

    return (PV_S0pp - PV_S0mm) / (*PtrS0 * 2 * *epsilon);  // Delta is the change in price divided by change in S0
}

/**
 * @brief Function to calculate the vega of an option using the CRR binomial model.
 * @param PtrS0 Pointer to initial stock price.
 * @param PtrK Pointer to strike price.
 * @param PtrSigma Pointer to volatility (sigma).
 * @param PtrT Pointer to time to expiry.
 * @param PtrR Pointer to risk-free rate.
 * @param PtrN Pointer to number of steps.
 * @param epsilon Small bump value for volatility.
 * @param Payoff Payoff function (e.g., call or put option).
 * @return Vega value.
 */
double CRR_vega(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrK,
    unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrT, unique_ptr<double>& PtrR, unique_ptr<int>& PtrN,
    double epsilon,
    const function<double(double, double)> &Payoff,
    const function<void(
        unique_ptr<double>&, unique_ptr<double>&, unique_ptr<double>&,
        unique_ptr<double>&, unique_ptr<double>&, unique_ptr<int>&
    )> &SetUDfromSigma
) {
    const double bump_ratio = epsilon / 1.0;
    auto nu = make_unique<double>(GetNuCentroid(*PtrS0, *PtrK, *PtrN, *PtrT));

    unique_ptr<double> PtrUpp, PtrDpp;
    unique_ptr<double> PtrSigmapp = make_unique<double>(*PtrSigma * (1. + bump_ratio));
    SetUDfromSigma(PtrUpp, PtrDpp, nu, PtrSigmapp, PtrT, PtrN);
    double PVpp = CRR_pv(PtrS0, PtrK, PtrUpp, PtrDpp, PtrR, PtrN, Payoff);

    unique_ptr<double> PtrUmm, PtrDmm;
    unique_ptr<double> PtrSigmamm = make_unique<double>(*PtrSigma * (1. - bump_ratio));
    SetUDfromSigma(PtrUmm, PtrDmm, nu, PtrSigmamm, PtrT, PtrN);
    double PVmm = CRR_pv(PtrS0, PtrK, PtrUmm, PtrDmm, PtrR, PtrN, Payoff);

    return (PVpp - PVmm) / (*PtrSigma * 2 * bump_ratio);  // Vega is the change in price divided by change in volatility
}

double NewtonSymb(int N, int i) {
    if (i<0 || i>N) return 0.0;

    double result = 1;
    for (int k=1; k<=i; k++) result *= static_cast<double>(N - i + k) / k;
    return result;
}

double Analytic_pv(
    unique_ptr<double>& S0, unique_ptr<double>& U, unique_ptr<double>& D, unique_ptr<double>& R,
    unique_ptr<int>& N, unique_ptr<double>& K,
    const function<double(double, double)>& Payoff
) {
    double q = riskNeutralProb(*U,*D,*R);
    vector<double> PDF(*N+1);
    double PDF_Sum = 0.0;

    auto* Price = new double[*N+1];
    double Sum = 0.0;
    for (int i=0; i<=*N; i++) {
        Price[i] = Payoff(S(*S0, *U, *D, *N, i), *K);
    }
    for (int j=0; j<=*N; j++) {
        PDF[j] = NewtonSymb(*N, j) * pow(q, j) * pow(1-q, *N-j);
        PDF_Sum += PDF[j];
        // cout << "j = " << j << ", PDF[j] = " << PDF[j] << endl;

        Sum += (PDF[j] * Price[j]);
    }
    // cout << "PDF_Sum = " << PDF_Sum << endl;

    double result = Sum/pow(1+*R,*N);
    delete[] Price;
    return result;
}

double Analytic_pv(
    unique_ptr<double>& S0, unique_ptr<double>& U, unique_ptr<double>& D, unique_ptr<double>& R,
    unique_ptr<int>& N, unique_ptr<double>& K1, unique_ptr<double>& K2,
    const function<double(double, double, double)>& Payoff
) {
    double q = riskNeutralProb(*U,*D,*R);
    vector<double> PDF(*N+1);
    double PDF_Sum = 0.0;

    auto* Price = new double[*N+1];
    double Sum = 0.0;
    for (int i=0; i<=*N; i++) {
        Price[i] = Payoff(S(*S0, *U, *D, *N, i), *K1, *K2);
    }
    for (int j=0; j<=*N; j++) {
        PDF[j] = NewtonSymb(*N, j) * pow(q, j) * pow(1-q, *N-j);
        PDF_Sum += PDF[j];
        // cout << "j = " << j << ", PDF[j] = " << PDF[j] << endl;

        Sum += (PDF[j] * Price[j]);
    }
    // cout << "PDF_Sum = " << PDF_Sum << endl;

    double result = Sum/pow(1+*R,*N);
    delete[] Price;
    return result;
}

double BS_EurCallOption_pv(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrR,
    unique_ptr<double>& PtrT, unique_ptr<double>& PtrK
) {
    // https://stats.stackexchange.com/questions/187828/how-are-the-error-function-and-standard-normal-distribution-function-related
    const double r = log(1 + *PtrR) / *PtrT; // risk-free Rate exponent

    const double d1 = (log(*PtrS0 / *PtrK) + (r + 0.5 * pow(*PtrSigma, 2.))* *PtrT) / (*PtrSigma * sqrt(*PtrT));
    const double Nd1 = 0.5 + erf(d1/sqrt(2))/2;

    const double d2 = d1 - *PtrSigma * sqrt(*PtrT); // (log(*PtrS0 / *PtrK) + (r - 0.5 * pow(*PtrSigma, 2.))* *PtrT) / (*PtrSigma * sqrt(*PtrT));
    const double Nd2 = 0.5 + erf(d2/sqrt(2))/2;

    const double result = *PtrS0 * Nd1 - *PtrK * exp(-r * *PtrT) * Nd2;

    return result;
}

double BS_EurPutOption_pv(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrR,
    unique_ptr<double>& PtrT, unique_ptr<double>& PtrK
) {
    // https://stats.stackexchange.com/questions/187828/how-are-the-error-function-and-standard-normal-distribution-function-related
    const double r = log(1 + *PtrR) / *PtrT; // risk-free Rate exponent

    const double d1 = (log(*PtrS0 / *PtrK) + (r + 0.5 * pow(*PtrSigma, 2.))* *PtrT) / (*PtrSigma * sqrt(*PtrT));
    const double Nmd1 = 0.5 + erf(-d1/sqrt(2))/2;

    const double d2 =  d1 - *PtrSigma * sqrt(*PtrT);  //(log(*PtrS0 / *PtrK) + (r - 0.5 * pow(*PtrSigma, 2.))* *PtrT) / (*PtrSigma * sqrt(*PtrT));
    const double Nmd2 = 0.5 + erf(-d2/sqrt(2))/2;

    const double result = *PtrK * exp(-r * *PtrT) * Nmd2 - *PtrS0 * Nmd1;

    return result;
}