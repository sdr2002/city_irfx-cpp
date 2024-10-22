#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <iomanip>
#include "Options03.h"
#include "BinModel01Sptr.h"

using namespace std;

void print_pv_and_greeks(
    const vector<double>& S0_range,
    const vector<double>& pvs_Analytic, const vector<double>& pvs_CRR,
    const vector<double>& deltas, const vector<double>& vegas
) {
    cout << setw(10) << "S0"
         << setw(20) << "PV_Analytical"
         << setw(20) << "PV_CRR"
         << setw(20) << "Delta"
         << setw(20) << "Vega" << endl;
    cout << string(90, '-') << endl;

    for (size_t i = 0; i < S0_range.size(); ++i) {
        cout << setw(10) << S0_range[i]
             << setw(20) << pvs_Analytic[i]
             << setw(20) << pvs_CRR[i]
             << setw(20) << deltas[i]
             << setw(20) << vegas[i]
             << endl;
    }
}

const double Atol = 1e-12;

void set_S0_range(vector<double>& S0_range, unique_ptr<double>& K1, unique_ptr<double>& K2) {
    double S0 = 0;
    while (S0 <= 200 + Atol) {
        S0_range.push_back(S0);
        if (S0 < *K1)
            S0 += 5;
        else if (S0 < *K1 + 5)
            S0 += 0.25;
        else if (S0 < *K2 - 5)
            S0 += 5;
        else if (S0 < *K2)
            S0 += 0.25;
        else
            S0 += 5;
    }
}

int main() {
    // Parameters as unique_ptr
    auto PtrK1 = make_unique<double>(150);  // Low barrier
    auto PtrK2 = make_unique<double>(50);  // Top barrier
    if (*PtrK1 > *PtrK2)
        swap(PtrK1, PtrK2);

    auto sigma = make_unique<double>(0.1);  // Annual volatility (10%)
    auto N = make_unique<int>(52);  // Number of steps (250BD/yr)
    auto R = make_unique<double>(0);  // Risk-free rate (0%)
    auto T = make_unique<double>(1.);  // Time to expiry in years
    auto epsilon = make_unique<double>(0.01);  // Small bump for delta and vega calculation

    // Calculating U and D based on sigma
    unique_ptr<double> PtrU, PtrD;

    // Payoff function for European Call Option
    function Pfn = DoubleKnockOutPayoff;

    // Range of strike prices S0 from 0 to 200
    vector<double> S0_range;
    set_S0_range(S0_range, PtrK1, PtrK2);

    vector<double> pvs_Analytic;
    vector<double> pvs_CRR;
    vector<double> deltas;
    vector<double> vegas;

    for (const double S0: S0_range) {
        auto PtrS0 = make_unique<double>(S0);

        SetUDfromVolatilityBsWith0Nu(PtrU, PtrD, sigma, T, N); // TODO We have 2K, so can't make Nu...
        // auto PtrNu = make_unique<double>(GetNuCentroid(*PtrS0, 100, *N, *T));
        // SetUDfromVolatilityBsWithNu(PtrU, PtrD, PtrNu, sigma, T, N);

        double pv_Analytic = Analytic_pv(PtrS0, PtrU, PtrD, R, N, PtrK1, PtrK2, Pfn);
        pvs_Analytic.push_back(pv_Analytic);
        double pv_CRR = CRR_pv(PtrS0, PtrK1, PtrK2, PtrU, PtrD, R, N, Pfn);
        pvs_CRR.push_back(pv_CRR);

        double delta = CRR_delta(PtrS0, PtrK1, PtrK2, PtrU, PtrD, R, N, epsilon, Pfn);
        deltas.push_back(delta);

        vegas.push_back(0.0);
    }

    // Output PV and delta
    print_pv_and_greeks(S0_range, pvs_Analytic, pvs_CRR, deltas, vegas);

    return 0;
}