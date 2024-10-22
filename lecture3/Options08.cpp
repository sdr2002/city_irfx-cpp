//
// Created by sdr2002 on 29/10/24.
//

#include "Options08.h"

#include <iostream>
#include <vector>

using namespace std;

double EurOption::PriceByCRR(BinModel Model) {
    double q=Model.RiskNeutProb();
    int N=GetN();
    vector<double> Price(N+1);
    for (int i=0; i<=N; i++) {
        Price[i] = Payoff(Model.S(N, i));
    }
    for (int n=N-1; n>=0; n--) {
        for (int i=0; i<=n; i++) {
            Price[i] = (q * Price[i+1] + (1-q) * Price[i]) / (1 + Model.GetR());
        }
    }
    return Price[0];
}

double AmOption::PriceBySnell(BinModel Model)
{
    double q=Model.RiskNeutProb();
    int N=GetN();
    vector<double> Price(N+1);
    for (int i=0; i<=N; i++) {
        Price[i] = Payoff(Model.S(N, i));
    }
    for (int n = N - 1; n >= 0; n--)
    {
        for (int i = 0; i <= n; i++)
        {
            const double ContVal = (q * Price[i + 1] + (1 - q) * Price[i]) / (1 + Model.GetR());
            const double ExerciseVal = Payoff(Model.S(n, i));
            Price[i] = max(ExerciseVal, ContVal);
        }
    }
    return Price[0];
}

int Call::GetInputGridParameters()
{
    cout << "Enter call option data:" << endl;
    int N;
    cout << "Enter steps to expiry N: "; cin >> N;
    EurOption::SetN(N); AmOption::SetN(N);
    cout << "Enter strike price K: "; cin >> K;
    cout << endl;
    return 0;
}

int Call::GetDefaultGridParameters() {
    cout << "Getting default data as N=50, K=70" << endl;
    EurOption::SetN(50); AmOption::SetN(50);
    SetK(110);
    return 0;
}

double Call::Payoff(double z)
{
    if (z > K) return z - K;
    return 0.0;
}

int Put::GetInputGridParameters()
{
    cout << "Enter put option data:" << endl;
    int N;
    cout << "Enter steps to expiry N: "; cin >> N;
    EurOption::SetN(N); AmOption::SetN(N);
    cout << "Enter strike price K: "; cin >> K;
    cout << endl;
    return 0;
}

int Put::GetDefaultGridParameters() {
    cout << "Getting default data as N=50, K=130" << endl;
    EurOption::SetN(50); AmOption::SetN(50);
    SetK(130);
    return 0;
}

double Put::Payoff(double z)
{
    if (z < K) return K - z;
    return 0.0;
}
