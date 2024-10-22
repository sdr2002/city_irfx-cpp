#include <iostream>
#include <cmath>
using namespace std;

int main() {
    double S0, U, D, R;
    // Entering data
    cout << "Enter S0: "; cin >> S0;   // 100
    cout << "Enter U: "; cin >> U;     // 0.05
    cout << "Enter D: "; cin >> D;     // -0.05
    cout << "Enter R: "; cin >> R;     // 0.1 - Can't be greater than U=0.05!

    // 1. Make sure S0 > 0, (1+U) > 0, (1+D) > 0, (1+R) > 0
    if (S0 <= 0 || U <= -1.0 || D <= -1.0 || R <= -1.0 || U <= D)
    {
        cout << "Illegal data ranges" << endl;
        cout << "Terminating program" << endl;
        return 1;
    }

    // 2. Checking for arbitrage: D < R < U; q > 1, or q < 0; q = (R-D)/(U-D)
    if (R <= D || R >= U)
    {
        cout << "Arbitrage exists" << endl;
        cout << "Terminating program" << endl;
        return 1;
    }

    cout << "Input data checked" << endl;
    cout << "There is no Arbitrage" << endl;

    // Compute the risk-neutral probability
    double q = (R - D) / (U - D);
    cout << "Risk-neutral probability q = " << q << endl;

    // Compute stock price at n=3, i=2
    int n = 3; int i = 2;
    cout << "S(" << n << "," << i << ") = " << S0 * pow(1+U, i) * pow(1+D, n-i) << endl;

    char x; cin >> x;

    return 0;
}
