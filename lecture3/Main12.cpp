#include "../lecture2/BinModel02.h"
#include "Options07.h"
#include <iostream>

using namespace std;

int main()
{
    BinModel Model;

    if (Model.GetDefaultGridParameters() == 1) return 1;

    Call Option1;
    Option1.GetDefaultGridParameters();
    cout << "European call option price = "
         << Option1.PriceByCRR(Model)
         << endl;
    cout << "American call option price = "
         << Option1.PriceBySnell(Model)
         << endl << endl;

    Put Option2;
    Option2.GetDefaultGridParameters();
    cout << "European put option price = "
         << Option2.PriceByCRR(Model)
         << endl;
    cout << "American put option price = "
         << Option2.PriceBySnell(Model)
         << endl << endl;

    return 0;
}
