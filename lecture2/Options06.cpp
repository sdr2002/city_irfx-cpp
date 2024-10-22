#include "Options06.h"
#include "BinModel02.h"
#include <iostream>
#include <cmath>
using namespace std;

double EurOption::PriceByCRR(BinModel Model) {
    double q = Model.RiskNeutProb();
    double Price[N + 1];
    for (int i = 0; i <= N; i++) {
        Price[i] = Payoff(Model.S(N, i));
    }
    for (int n = N - 1; n >= 0; n--) {
        for (int i = 0; i <= n; i++) {
            Price[i] = (q * Price[i + 1] + (1 - q) * Price[i]) / (1 + Model.GetR());
        }
    }
    return Price[0];
}

double NewtonSymb(int N, int i) {
    if (i<0 || i>N) return 0.0;

    double result = 1;
    for (int k=1; k<=i; k++) result *= static_cast<double>(N - i + k) / k;
    return result;
}

double EurOption::PriceByAnalyticBinomial(BinModel Model) {
    double q = Model.RiskNeutProb();
    auto* PDF = new double[N+1];
    double PDF_Sum = 0.0;

    auto* Price = new double[N+1];
    double Sum = 0.0;
    for (int i=0; i<=N; i++) {
        Price[i] = Payoff(Model.S(N, i));
    }
    for (int j=0; j<=N; j++) {
        PDF[j] = NewtonSymb(N, j) * pow(q, j) * pow(1-q, N-j);
        PDF_Sum += PDF[j];
        Sum += (PDF[j] * Price[j]);
    }

    double result = Sum/pow(1 + Model.GetR(), N);
    delete[] Price;
    return result;
}

// Adding DeltaByCRR method to calculate the delta greek
double EurOption::DeltaByCRR(BinModel Model, double epsilon) {
    // Calculate the price for S*(1+epsilon)
    const double OriginalS0 = Model.GetS0();
    Model.SetS0(OriginalS0 * (1 + epsilon));
    double PriceUp = PriceByCRR(Model);

    // Reset S0 and calculate the price for S*(1-epsilon)
    Model.SetS0(OriginalS0 * (1 - epsilon));
    double PriceDown = PriceByCRR(Model);

    // Reset S0 to its original value
    Model.SetS0(OriginalS0);

    // Calculate and return delta
    return (PriceUp - PriceDown) / (OriginalS0 * 2 * epsilon);
}

int EurCall::GetInputGridParameters() {
    cout << "Enter call option data:" << endl;
    int N;
    cout << "Enter steps to expiry N: "; cin >> N;
    SetN(N);
    cout << "Enter strike price K: "; cin >> K;
    cout << endl;
    return 0;
}

int EurCall::GetDefaultGridParameters() {
    cout << "Getting default data as N=10, K=100" << endl;
    SetN(10);
    K = 100;
    return 0;
}

double EurCall::Payoff(double z) {
    if (z > K) return z - K;
    return 0.0;
}

int EurPut::GetInputGridParameters() {
    cout << "Enter put option data:" << endl;
    int N;
    cout << "Enter steps to expiry N: "; cin >> N;
    SetN(N);
    cout << "Enter strike price K: "; cin >> K;
    cout << endl;
    return 0;
}

int EurPut::GetDefaultGridParameters() {
    cout << "Getting default data as N=10, K=100" << endl;
    SetN(10);
    K = 100;
    return 0;
}

double EurPut::Payoff(double z) {
    if (z < K) return K - z;
    return 0.0;
}

int EurDoubleKnockOut::GetInputGridParameters() {
    cout << "Enter DoubleKnockOut option data:" << endl;
    int N;
    cout << "Enter steps to expiry N: "; cin >> N;
    SetN(N);
    cout << "Enter low-barrier price K: "; cin >> Kl;
    cout << "Enter high-barrier price K2: "; cin >> Kh;
    cout << endl;
    return 0;
}

int EurDoubleKnockOut::GetDefaultGridParameters() {
    cout << "Getting default data as N=10, K1=50, K2=150" << endl;
    SetN(10);
    Kl = 50;
    Kh = 150;
    return 0;
}

double EurDoubleKnockOut::Payoff(double z) {
    return (Kl < z) && (z < Kh);
}

// TODO put PriceByBlackScholes and Other U/D <-> Sigma tools : from Options03 in lecture1