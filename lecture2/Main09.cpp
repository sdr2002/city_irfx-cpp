#include "BinModel02.h"
#include "Options05.h"
#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    cout << "At Main09.cpp" << endl;

    BinModel Model;

    if (Model.GetInputGridParameters() == 1) return 1;

    EurCall Option1;
    Option1.GetInputGridParameters();
    cout << "European call option price = "
         << Option1.PriceByCRR(Model, Option1.GetK())
         << endl << endl;

    EurPut Option2;
    Option2.GetInputGridParameters();
    cout << "European put option price = "
         << Option2.PriceByCRR(Model, Option2.GetK())
         << endl << endl;

    return 0;
}
