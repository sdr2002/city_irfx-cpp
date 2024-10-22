//
// Created by sdr2002 on 10/10/24.
//

#ifndef BINMODEL01_H
#define BINMODEL01_H

//computing riskNeutralProb
double riskNeutralProb(double U, double D, double R);

//computing the stockPrice at node (n, i)
double S(double S0, double U, double D, int n, int i);

//inputting, displaying and checking the model data
int GetInputGridParameters(double &S0, double &U, double &D, double &R);

#endif //BINMODEL01_H
