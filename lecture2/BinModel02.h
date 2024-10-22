#ifndef BinModel2_h
#define BinModel2_h

class BinModel
{
private:
    double S0; // Initial stock price
    double U;  // Up factor
    double D;  // Down factor
    double R;  // Risk-free rate

    // Shared method to check parameters
    int checkData();

public:
    // Constructor
    BinModel();

    // Method to compute the risk-neutral probability
    double RiskNeutProb();

    // Method to compute stock price at node (n, i)
    double S(int n, int i);

    // Method to get input data from the user
    int GetInputGridParameters();

    // Method to set default values
    int GetDefaultGridParameters();

    // Getters and Setters
    double GetS0();
    void SetS0(double S0new);
    double GetR();
};

#endif
