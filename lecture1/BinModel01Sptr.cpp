#include <iostream>
#include <cmath>
#include <memory>
using namespace std;

// Function to get input data
int GetInputGridParameters(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrR
){
    double tempS0, tempU, tempD, tempR;
    // Entering data
    cout << "Enter S0: "; cin >> tempS0;   // 100
    cout << "Enter U: "; cin >> tempU;     // 0.9
    cout << "Enter D: "; cin >> tempD;     // -0.3
    cout << "Enter R: "; cin >> tempR;     // 0.1

    // 1. Make sure S0 > 0, (1+U) > 0, (1+D) > 0, (1+R) > 0, U < D, otherwise interchange U <=> D
    if (tempS0 <= 0 || tempU <= -1.0 || tempD <= -1.0 || tempR <= -1.0 || tempU <= tempD)
    {
        cout << "Illegal data ranges" << endl;
        cout << "Terminating program" << endl;
        return 1;
    }

    // 2. Checking for arbitrage: D < R < U; q > 1, or q < 0; q = (R-D)/(U-D)
    if (tempR <= tempD || tempR >= tempU)
    {
        cout << "Arbitrage exists" << endl;
        cout << "Terminating program" << endl;
        return 1;
    }

    cout << "Input data checked" << endl;
    cout << "There is no Arbitrage" << endl;

    PtrS0 = make_unique<double>(tempS0);
    PtrU = make_unique<double>(tempU);
    PtrD = make_unique<double>(tempD);
    PtrR = make_unique<double>(tempR);

    return 0;
}

int GetInputGridParameters(
    unique_ptr<double>& PtrS0, unique_ptr<double>& PtrVoltBS, unique_ptr<double>& PtrR
){
    double tempS0, tempVoltBS, tempR;
    // Entering data
    cout << "Enter S0: "; cin >> tempS0;   // 100
    cout << "Enter VoltBS: "; cin >> tempVoltBS;     // 0.1
    cout << "Enter R: "; cin >> tempR;     // 0.1

    PtrS0 = make_unique<double>(tempS0);
    PtrVoltBS = make_unique<double>(tempVoltBS);
    PtrR = make_unique<double>(tempR);

    return 0;
}

// Function to compute the risk-neutral probability
double riskNeutralProb(double U, double D, double R)
{
    double q = (R - D) / (U - D);
    // cout << "    riskNeutralProb = " << q << endl;
    return q;
}

// Function to compute stock price at node (n, i)
double S(double S0, double U, double D, int n, int i)
{
    return S0 * pow(1 + U, i) * pow(1 + D, n - i);
}

int mainBinModel01Sptr()  // mainBinModel01Sptr
{
    unique_ptr<double> S0, U, D, R;

    // Get input data
    if (GetInputGridParameters(S0, U, D, R) == 1) return 1;

    // Compute the risk-neutral probability
    double q = riskNeutralProb(*U, *D, *R);
    cout << "Risk-neutral probability q = " << q << endl;

    // Compute stock price at n = 3, i = 2
    int n = 3; int i = 2;
    cout << "S(" << n << "," << i << ") = " << S(*S0, *U, *D, n, i) << endl;

    char x; cin >> x;

    return 0;
}

/**
 * @brief Calculates up and down move factors based on volatility using a special case formula (U * D = 1).
 * @param PtrU Pointer to store the up move factor.
 * @param PtrD Pointer to store the down move factor.
 * @param PtrNu Pointer to store (1+U) * (1+D) - or u * d - constant.
 * @param PtrSigma Pointer to the volatility (sigma).
 * @param PtrT Pointer to the time to expiry.
 * @param PtrN Pointer to the number of steps.
 * @return 0 if successful.
 */
void SetUDfromVolatilityBsWithNu(
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrNu,
    unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrT, unique_ptr<int>& PtrN
) {
    /* u * d = exp(2 * nu * dT) model for U, D, Sigma relationship: see p.50 or Emilio's Binomial model slides
     * u = exp(nu * dT + sigma * sqrt(dT))
     * d = exp(nu * dT - sigma * sqrt(dT))
     * */
    const double dT = *PtrT / *PtrN;
    const double nuXdT = *PtrNu * dT;  //u * d = exp(nu * dT)
    const double sigmaXsqrtdT = *PtrSigma * sqrt(dT);

    const double rawU = exp(nuXdT + sigmaXsqrtdT) - 1;  // Up move rate per step
    const double rawD = exp(nuXdT - sigmaXsqrtdT) - 1;  // Down move rate per step

    PtrU = make_unique<double>(rawU);
    PtrD = make_unique<double>(rawD);
}

void SetUDfromVolatilityBsWith0Nu(
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD,
    unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrT, unique_ptr<int>& PtrN
) {
    auto zeroNu = make_unique<double>(0);
    SetUDfromVolatilityBsWithNu(
        PtrU, PtrD, zeroNu, PtrSigma, PtrT, PtrN
    );
}

void SetUDfromVolatilityBsWith0Nu(
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrNu,
    unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrT, unique_ptr<int>& PtrN
) {
    SetUDfromVolatilityBsWith0Nu(
        PtrU, PtrD, PtrSigma, PtrT, PtrN
    );
}

double GetNuCentroid(double S0, double K, int N, double T) {
    double dT = T/N;
    return -1.0 / (N * dT)  * log(S0 / K);  // -1/(N * dT) *(ln(S0) - ln(K))
}