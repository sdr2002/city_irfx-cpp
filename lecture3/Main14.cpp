#include "BinLattice02.h"
#include "../lecture2/BinModel02.h"
#include "Options09.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
    BinModel Model;

    // if (Model.GetInputGridParameters() == 1) return 1;
    if (Model.GetDefaultGridParameters() == 1) return 1;

    Put Option;

    // Option.GetInputGridParameters();
    Option.GetDefaultGridParameters();

    const double eurPutPrice = Option.PriceByCRR(Model);
    cout << "European " << to_string(Option) << " price: " << eurPutPrice << endl << endl;

    BinLattice<double> PriceTree("PriceTree");
    BinLattice<bool> StoppingTree("StoppingTree");
    const double amPutPrice = Option.PriceBySnell(Model, PriceTree, StoppingTree);
    cout << "American " << to_string(Option) << " price: " << amPutPrice << endl << endl;

    PriceTree.Display();
    StoppingTree.Display();

    return 0;
}
