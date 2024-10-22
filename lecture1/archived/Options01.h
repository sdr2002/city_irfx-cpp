//
// Created by sdr2002 on 10/10/24.
//

#ifndef OPTIONS01_H
#define OPTIONS01_H

int GetInputGridParameters(int&N, double& K);

double PriceByCRR(double S0, double U, double D, double R, int N, double K);

double CallPayoff(double z, double K);

double PutPayoff(double z, double K);

double NewtonSymb(int N, int i);

double PriceAnalytic(double S0, double U, double D, double R, int N, double K);

#endif //OPTIONS01_H
