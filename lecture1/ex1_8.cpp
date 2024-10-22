#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <iomanip>
#include "Options03.h"
#include "BinModel01Sptr.h"

using namespace std;

/**
 * @brief Function to print the option price (PV), delta, and vega for various strike prices.
 * @param S0_range Vector of strike prices.
 * @param pvs_Analytic Vector of option prices in PV by analytical binomial formula
 * @param pvs_CRR Vector of option prices in PV by CRR model.
 * @param deltas Vector of delta values.
 * @param vegas Vector of vega values.
 */
void print_pv_and_greeks(
    const vector<double>& S0_range,
    const vector<double>& pvs_Analytic, const vector<double>& pvs_CRR, const vector<double>& pvs_BS,
    const vector<double>& deltas, const vector<double>& vegas
) {
    cout << setw(10) << "K"
         << setw(20) << "PV_Analytical"
         << setw(20) << "PV_CRR"
         << setw(20) << "PV_BS"
         << setw(20) << "Delta"
         << setw(20) << "Vega" << endl;
    cout << string(110, '-') << endl;

    for (size_t i = 0; i < S0_range.size(); ++i) {
        cout << setw(10) << S0_range[i]
             << setw(20) << pvs_Analytic[i]
             << setw(20) << pvs_CRR[i]
             << setw(20) << pvs_BS[i]
             << setw(20) << deltas[i]
             << setw(20) << vegas[i]
             << endl;
    }
}

const double Atol = 1e-12;

void set_K_range(vector<double>& K_range) {
    double K = 50;
    while (K <= 150 + Atol) {
        K_range.push_back(K);
        if (K < 95)
            K += 5;
        else if (K < 105)
            K += 0.25;
        else
            K += 5;
    }
}

int main() {
    // Parameters as unique_ptr
    auto S0 = make_unique<double>(100);  // Initial stock price
    auto sigma = make_unique<double>(0.1);  // Annual volatility. 0.1 means volatility=S*0.1
    auto N = make_unique<int>(50);  // Number of steps (250BD/yr)
    auto T = make_unique<double>(1);  // Time to expiry in years
    auto epsilon = make_unique<double>(0.0001);  // Small bump for delta and vega calculation. 0.01 = 1% change

    auto R = make_unique<double>(0.045);  // Risk-free rate per annum. 0.01 = 1%
    unique_ptr<double> PtrUnitR = make_unique<double>(exp(log(1 + *R) / (*N / *T)) - 1);

    // Calculating U and D based on sigma
    unique_ptr<double> PtrU, PtrD;
    function SetUDfromVolt = SetUDfromVolatilityBsWithNu;

    // Payoff function for European Call Option
    // TODO Make the result of DigitalCallPayoff.Vega realistic
    function Pfn = EuropeanCallPayoff;
    auto PfnTar = Pfn.target<double(*)(double, double)>();

    const bool isPfnEuropeanCallPayoff = *PfnTar == &EuropeanCallPayoff; // EuropeanCallPayoff also works
    if (isPfnEuropeanCallPayoff) {
        cout << "Payoff fn looks like EuropeanCallPayoff" << endl;
    }
    const bool isPfnEuropeanPutPayoff = *PfnTar == &EuropeanPutPayoff; // EuropeanCallPayoff also works
    if (isPfnEuropeanPutPayoff) {
        cout << "Payoff fn looks like EuropeanPutPayoff" << endl;
    }

    // Range of strike prices K from 50 to 150
    vector<double> K_range;
    set_K_range(K_range);
    // Add values with step size 5 for K from 50 to 90

    vector<double> pvs_Analytic;
    vector<double> pvs_CRR;
    vector<double> pvs_BS;
    vector<double> deltas;
    vector<double> vegas;

    // Calculate the option price, delta, and vega for each strike price K
    for (double K : K_range) {
        auto nu = make_unique<double>(GetNuCentroid(*S0, K, *N, *T));
        SetUDfromVolt(PtrU, PtrD, nu, sigma, T, N);

        auto PtrK = make_unique<double>(K);

        double pv_Analytic = Analytic_pv(S0, PtrU, PtrD, PtrUnitR, N, PtrK, Pfn);
        pvs_Analytic.push_back(pv_Analytic);
        double pv_CRR = CRR_pv(S0, PtrK, PtrU, PtrD, PtrUnitR, N, Pfn);
        pvs_CRR.push_back(pv_CRR);

        double pv_BS(0);
        if (isPfnEuropeanCallPayoff) {
            pv_BS = BS_EurCallOption_pv(S0, sigma, R, T, PtrK);  // ONLY if Pfn is EuropeanCallPayoff
        } else if (isPfnEuropeanPutPayoff) {
            pv_BS = BS_EurPutOption_pv(S0, sigma, R, T, PtrK);
        }
        pvs_BS.push_back(pv_BS);

        double delta = CRR_delta(S0, PtrK, PtrU, PtrD, PtrUnitR, N, epsilon, Pfn);
        deltas.push_back(delta);

        double vega = CRR_vega(S0, PtrK, sigma, T, PtrUnitR, N, *epsilon, Pfn, SetUDfromVolt);
        vegas.push_back(vega);
    }

    // Output PV and delta
    print_pv_and_greeks(K_range, pvs_Analytic, pvs_CRR, pvs_BS, deltas, vegas);

    return 0;
}


/* In C++, std::function::target provides access to the stored callable (such as a function pointer) within a std::function object. Here's a breakdown of how the function comparison works in your example:

    How std::function::target Works:
    What target() Returns:

    When you call Pfn.target<double(*)(double, double)>();, you're asking std::function to return a pointer to the internally stored function pointer if it's of the specified type, double(*)(double, double).
    If the std::function actually holds a function pointer of the specified type, it returns a pointer to the function pointer. Otherwise, it returns nullptr.
    Therefore, PfnTar is of type double(**)(double, double), which means:

    It's a pointer to a function pointer of the type double(*)(double, double).
    Dereferencing PfnTar:

    *PfnTar dereferences PfnTar, giving us the actual function pointer stored inside the std::function.
    For example, if Pfn holds a function pointer to EuropeanCallPayoff, then *PfnTar will hold the value &EuropeanCallPayoff.
    Comparing Function Pointers:

    Function pointers in C++ are simply memory addresses pointing to the function's entry point in memory. When you compare two function pointers, you're comparing whether both point to the same function.
    *PfnTar == EuropeanCallPayoff compares the function pointer extracted from Pfn with the pointer to the EuropeanCallPayoff function. If they point to the same function, the result is true.
    The following comparisons are valid:

    *PfnTar == EuropeanCallPayoff compares the dereferenced PfnTar with the EuropeanCallPayoff function.
    *PfnTar == &EuropeanCallPayoff also works because &EuropeanCallPayoff is the explicit address of the EuropeanCallPayoff function.
    Why This Works:
    Type Matching:
    When you create std::function<double(double, double)> Pfn = EuropeanCallPayoff;, it internally stores the pointer to EuropeanCallPayoff, which is of type double(*)(double, double).
    By calling Pfn.target<double(*)(double, double)>();, you retrieve the internal pointer to this function pointer.
    Since you're comparing function pointers, the actual comparison works because both point to the same memory address.
*/