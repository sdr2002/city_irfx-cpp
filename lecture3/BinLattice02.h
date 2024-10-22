//
// Created by sdr2002 on 29/10/24.
//

#ifndef BINLATTICE02_H
#define BINLATTICE02_H

#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <vector>

using namespace std;

template<typename T> class BinLattice {
    string name;

    int N;
    vector<vector<T>> Lattice;

    public:
        BinLattice(string name) {this->name = name;}

        void SetN(int N_) {
            N = N_;
            Lattice.resize(N+1);
            for (int n = 0; n < N+1; n++) {
                Lattice[n].resize(N+1);
            }
        }

        void SetNode(int n, int i, T x) {
            Lattice[n][i] = x;
        }

        T GetNode(int n, int i) {return Lattice[n][i];}

        void Display() {
            cout << "  " << this->name << ": " << endl;

            cout << setiosflags(ios::fixed) << setprecision(3);
            for (int n = 0; n < N+1; n++) {
                for (int i = 0; i <= N; i++) {
                    cout << setw(10) << GetNode(n, i);
                }
                cout << endl;
            }
            cout << endl;
        }
};

#endif //BINLATTICE02_H
