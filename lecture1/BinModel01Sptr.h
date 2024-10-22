//
// Created by sdr2002 on 10/10/24.
//

#ifndef BINMODEL01SPTR_H
#define BINMODEL01SPTR_H
#include <memory>  // For smart pointers

using namespace std;

//computing riskNeutralProb
double riskNeutralProb(double U, double D, double R);

//computing the stockPrice at node (n, i)
double S(double S0, double U, double D, int n, int i);

//inputting, displaying and checking the model data
int GetInputGridParameters(unique_ptr<double>& PtrS0, unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrR);

int GetInputGridParameters(unique_ptr<double>& PtrS0, unique_ptr<double>& PtrVoltBS, unique_ptr<double>& PtrR);

void SetUDfromVolatilityBsWithNu(
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrNu,
    unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrT, unique_ptr<int>& PtrN
);

void SetUDfromVolatilityBsWith0Nu(
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD,
    unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrT, unique_ptr<int>& PtrN
);

void SetUDfromVolatilityBsWith0Nu(
    unique_ptr<double>& PtrU, unique_ptr<double>& PtrD, unique_ptr<double>& PtrNu,
    unique_ptr<double>& PtrSigma, unique_ptr<double>& PtrT, unique_ptr<int>& PtrN
);

double GetNuCentroid(double S0, double K, int N, double T);

#endif //BINMODEL01SPTR_H
