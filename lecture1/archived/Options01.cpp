
#include <iostream>
#include <cmath>
#include "Options01.h"

#include <vector>

#include "BinModel01.h"

using namespace std;

int GetInputGridParameters(int &N, double &K) {
    cout << "Enter steps to expiry N: "; cin >> N;
    cout << "Enter strike price K: "; cin >> K;
    cout << endl;
    return 0;
}

double PriceByCRR(double S0, double U, double D, double R, int N, double K) {
    double q = riskNeutralProb(U,D,R);
    double Price[N+1];
    for (int i=0; i<=N; i++) {
        Price[i] = CallPayoff(S(S0, U, D, N, i), K);
    }
    for (int n= N-1; n>=0; n--) {
        for (int i=0; i<=n; i++) {
            Price[i] = (q * Price[i+1] + (1-q) * Price[i])/(1+R);
        }
    }
    return Price[0];
}

double CallPayoff(double z, double K) {
    if (z>K) return z-K;
    return 0.0;
}

double PutPayoff(double z, double K)
{
    if (z < K) return K - z;
    return 0.0;
}

double NewtonSymb(int N, int i) {
    if (i<0 || i>N) return 0.0;

    double result = 1;
    for (int k=1; k<=i; k++) result *= static_cast<double>(N - i + k) / k;
    return result;
}

double PriceAnalytic(double S0, double U, double D, double R, int N, double K) {
    double q = riskNeutralProb(U,D,R);
    vector<double> PDF(N+1);
    double PDF_Sum = 0.0;

    auto* Price = new double[N+1];
    double Sum = 0.0;
    for (int i=0; i<=N; i++) {
        Price[i] = CallPayoff(S(S0, U, D, N, i), K);
    }
    for (int j=0; j<=N; j++) {
        PDF[j] = NewtonSymb(N, j) * pow(q, j) * pow(1-q, N-j);
        PDF_Sum += PDF[j];
        cout << "j = " << j << ", PDF[j] = " << PDF[j] << endl;

        Sum += (PDF[j] * Price[j]);
    }
    cout << "PDF_Sum = " << PDF_Sum << endl;

    double result = Sum/pow(1+R,N);
    delete[] Price;
    return result;
}
