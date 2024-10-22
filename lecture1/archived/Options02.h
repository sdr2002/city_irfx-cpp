//
// Created by sdr2002 on 10/10/24.
//

#ifndef OPTIONS02_H
#define OPTIONS02_H

int GetInputGridParameters(int* PtrN, double* PtrK);

double PriceByCRR(double S0, double U, double D, double R, int N, double K);

double CallPayoff(double z, double K);

#endif //OPTIONS02_H
