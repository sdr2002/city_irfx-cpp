#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <iomanip>
#include <functional>

using namespace std;

/**
 * @brief Calculates up and down move factors based on volatility using a special case formula (U * D = 1).
 * @param PtrU Pointer to store the up move factor.
 * @param PtrD Pointer to store the down move factor.
 * @param PtrSigma Pointer to the volatility (sigma).
 * @param PtrT Pointer to the time to expiry.
 * @param PtrN Pointer to the number of steps.
 * @return 0 if successful.
 */
int UdFromVolatilityBsWith0v(
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD,
    unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrT, unique_ptr<int>& PtrN
) {
    const double rawU = exp(*PtrSigma * sqrt(*PtrT / *PtrN)) - 1;  // Up move per step
    const double rawD = exp(-*PtrSigma * sqrt(*PtrT / *PtrN)) - 1;  // Down move per step

    PtrU = make_unique<double>(rawU);
    PtrD = make_unique<double>(rawD);

    return 0;
}

/**
 * @brief Payoff function for European Call Option.
 * @param z Current stock price.
 * @param K Strike price.
 * @return Payoff value.
 */
double EuropeanCallPayoff(double z, double K) {
    return max(z - K, 0.0);
}

double DigitalCallPayoff(double z, double K)
{
    if (z > K) return 1.0;
    return 0.0;
}

/**
 * @brief Binomial CRR pricing model for European Call Option.
 * @param S0 Initial stock price.
 * @param K Strike price.
 * @param U Up move factor.
 * @param D Down move factor.
 * @param R Risk-free rate.
 * @param N Number of steps.
 * @param Payoff Payoff function (e.g., call or put option).
 * @return Option price.
 */
double CRR_european(
    double S0, double K, double U, double D, double R, int N, function<double(double, double)> Payoff
) {
    double q = (R - D) / (U - D);  // Risk-neutral probability
    vector<double> price(N + 1);

    // Calculate the payoff at maturity
    for (int i = 0; i <= N; ++i) {
        price[i] = Payoff(S0 * pow(1 + U, i) * pow(1 + D, N - i), K);
    }

    // Step backwards through the tree to calculate the option price
    for (int n = N - 1; n >= 0; --n) {
        for (int i = 0; i <= n; ++i) {
            price[i] = (q * price[i + 1] + (1 - q) * price[i]) / (1 + R);
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
    double S0, double K, double U, double D, double R, int N, double epsilon, function<double(double, double)> Payoff
) {
    double PV_S0pp = CRR_european(S0 + epsilon, K, U, D, R, N, Payoff);  // Price with bumped S0
    double PV_S0mm = CRR_european(S0 - epsilon, K, U, D, R, N, Payoff);  // Price with reduced S0
    return (PV_S0pp - PV_S0mm) / (2 * epsilon);  // Delta is the change in price divided by change in S0
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
    double epsilon, function<double(double, double)> Payoff
) {
    unique_ptr<double> PtrUpp, PtrDpp;
    unique_ptr<double> PtrSigmapp = make_unique<double>(*PtrSigma * (1. + epsilon/100.0));
    UdFromVolatilityBsWith0v(PtrUpp, PtrDpp, PtrSigmapp, PtrT, PtrN);
    double Upp = *PtrUpp;  // Up move per step
    double Dpp = *PtrDpp;  // Down move per step
    double PVpp = CRR_european(*PtrS0, *PtrK, Upp, Dpp, *PtrR, *PtrN, Payoff);

    unique_ptr<double> PtrUmm, PtrDmm;
    unique_ptr<double> PtrSigmamm = make_unique<double>(*PtrSigma * (1. - epsilon/100.0));
    UdFromVolatilityBsWith0v(PtrUmm, PtrDmm, PtrSigmamm, PtrT, PtrN);
    double Umm = *PtrUmm;  // Up move per step
    double Dmm = *PtrDmm;  // Down move per step
    double PVmm = CRR_european(*PtrS0, *PtrK, Umm, Dmm, *PtrR, *PtrN, Payoff);

    return (PVpp - PVmm) / (2 * epsilon/100.0);  // Vega is the change in price divided by change in volatility
}

/**
 * @brief Function to print the option price (PV), delta, and vega for various strike prices.
 * @param K_range Vector of strike prices.
 * @param option_pvs Vector of option prices (PV).
 * @param deltas Vector of delta values.
 * @param vegas Vector of vega values.
 */
void print_pv_and_greeks(
    const vector<double>& K_range, const vector<double>& option_pvs,
    const vector<double>& deltas, const vector<double>& vegas
) {
    cout << setw(10) << "K" << setw(20) << "Option Price" << setw(20) << "Delta" << setw(20) << "Vega" << endl;
    cout << string(70, '-') << endl;

    for (size_t i = 0; i < K_range.size(); ++i) {
        cout << setw(10) << K_range[i]
             << setw(20) << option_pvs[i]
             << setw(20) << deltas[i]
             << setw(20) << vegas[i]
             << endl;
    }
}

int main() {
    // Parameters as unique_ptr
    auto S0 = make_unique<double>(100);  // Initial stock price
    auto sigma = make_unique<double>(0.1);  // Annual volatility (10%)
    auto N = make_unique<int>(50);  // Number of steps (50 weeks)
    auto R = make_unique<double>(0);  // Risk-free rate (0%)
    auto T = make_unique<double>(1);  // Time to expiry in years
    auto epsilon = make_unique<double>(0.05);  // Small bump for delta and vega calculation

    // Calculating U and D based on sigma
    unique_ptr<double> PtrU, PtrD;
    UdFromVolatilityBsWith0v(PtrU, PtrD, sigma, T, N);
    double U = *PtrU;  // Up move per step
    double D = *PtrD;  // Down move per step

    // Range of strike prices K from 50 to 150
    vector<double> K_range;
    for (double K = 50; K <= 150; K += 1) {
        K_range.push_back(K);
    }

    vector<double> option_prices;
    vector<double> deltas;
    vector<double> vegas;

    // Payoff function for European Call Option
    function Pfn = EuropeanCallPayoff;

    // Calculate the option price, delta, and vega for each strike price K
    for (double K : K_range) {
        auto PtrK = make_unique<double>(K);
        double option_price = CRR_european(*S0, K, U, D, *R, *N, Pfn);
        option_prices.push_back(option_price);

        double delta = CRR_delta(*S0, K, U, D, *R, *N, *epsilon, Pfn);
        deltas.push_back(delta);

        double vega = CRR_vega(S0, PtrK, sigma, T, R, N, *epsilon, Pfn);
        vegas.push_back(vega);
    }

    // Output PV and delta
    print_pv_and_greeks(K_range, option_prices, deltas, vegas);

    return 0;
}
