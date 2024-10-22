//
// Created by sdr2002 on 29/10/24.
//

#ifndef BINLATTICE01_H
#define BINLATTICE01_H
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <vector>

using namespace std;

class BinLattice {
    int N;
    vector<vector<double>> Lattice;

    public:
        void SetN(int N_) {
            N = N_;
            Lattice.resize(N+1);
            for (int n = 0; n < N+1; n++) {
                Lattice[n].resize(N+1);
            }
        }

        void SetNode(int n, int i, double x) {
            Lattice[n][i] = x;
        }

        double GetNode(int n, int i) {return Lattice[n][i];}

        void Display() {
            cout << setiosflags(ios::fixed) << setprecision(3);
            for (int n = 0; n < N+1; n++) {
                for (int i = 0; i <= N; i++) {
                    cout << setw(7) << GetNode(n, i);
                }
                cout << endl;
            }
            cout << endl;
        }
};

#endif //BINLATTICE01_H
