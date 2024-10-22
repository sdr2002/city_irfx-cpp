#include "BinModel02.h"
#include "Options06.h"
#include <iostream>
#include <cmath>
using namespace std;

int main() {
    cout << "At Main10.cpp" << endl;

    BinModel Model;

    if (Model.GetDefaultGridParameters() == 1) return 1;

    EurCall Option1;
    Option1.GetDefaultGridParameters();
    cout << "European call option price = " << Option1.PriceByCRR(Model) << endl;
    cout << "European call option delta = " << Option1.DeltaByCRR(Model, 0.01) << endl;

    cout << "European call option price by AnalyticBinomial formula = "
         << Option1.PriceByAnalyticBinomial(Model)
         << endl;

    EurPut Option2;
    Option2.GetDefaultGridParameters();
    cout << "European put option price = "
            << Option2.PriceByCRR(Model)
            << endl << endl;

    EurDoubleKnockOut Option3;
    Option3.GetDefaultGridParameters();
    cout << "EurDoubleKnockOut option price = "
            << Option3.PriceByCRR(Model)
            << endl << endl;

    return 0;
}
