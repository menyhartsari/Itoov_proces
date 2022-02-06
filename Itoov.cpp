#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <iostream>
#include <fstream>

class Trajektoria {
private:
    std::vector<double> x;
    std::vector<double> y;
public:
    Trajektoria() {};
    Trajektoria(std::vector<double> X, std::vector<double> Y) { x = X; y = Y; }
    std::vector<double> getX() {
        return x;
    }
    std::vector<double> getY() {
        return y;
    }
};

double rand_gen() {
    return ((double)(rand()) + 1.) / ((double)(RAND_MAX)+1.);
}
double normalRandom() {
    double v1 = rand_gen();
    double v2 = rand_gen();
    return cos(2 * M_PI * v2) * sqrt(-2. * log(v1));
}

Trajektoria generuj(std::vector<double> x, Trajektoria T) {
    std::vector<double> dS;
    double krok = x[1] - x[0];
    for (int i = 0; i <= x.size() - 1; i++) {
        dS.push_back(sqrt(krok) * normalRandom());
    }
    double S0 = 0;
    std::vector<double> B;
    for (int i = 0; i <= x.size() - 1; i++) {
        B.push_back(S0);
        S0 += dS[i];
    }
    Trajektoria P(x, B);
    T = P;
    return P;
}

void ulozenie(std::vector<Trajektoria> T, int d, int q) {
    std::fstream out;
    if (q == 0) {
        out.open("Trajektorie.txt", std::ios::out);
    }
    if (q == 1) {
        out.open("Ito_1.txt", std::ios::out);
    }
    if (q == 2) {
        out.open("Ito_2.txt", std::ios::out);
    }
    if (out.is_open()) {
        out << "x,y,typ" << std::endl;
        for (int i = 0; i <= d; i++) {
            for (int j = 0; j <= T[i].getX().size() - 1; j++) {
                out << T[i].getX()[j] << "," << T[i].getY()[j] << "," << i << std::endl;
            }
        }
        out.close();
    }
    else {
        std::cout << "neotvorilo subor";
    }
}

Trajektoria ito_1sposob(std::vector<double> x, Trajektoria T) {
   std::vector<double> I;
    for (int i = 0; i <= T.getX().size() - 1; i++) {
        I.push_back(pow(x.at(i), 3) * pow(T.getY().at(i), 4));
    }
    Trajektoria P(x, I);
    T = P;
    return T;
}
Trajektoria ito_2sposob(std::vector<double> cas, Trajektoria T) {
    std::vector<double> I;
    std::vector<double> R;
    std::vector<double> S(T.getX().size());

    I.push_back(0);
    R.push_back(0);
    for (int i = 1; i <= T.getY().size() - 1; i++) {
        R.push_back((3 * pow(cas.at(i), 2) * pow(T.getY().at(i), 4) + 6 * pow(cas.at(i), 3) * pow(T.getY().at(i), 2)) * (cas.at(i) - cas.at(i - 1)) + R.at(i - 1));
        I.push_back((4 * pow(cas.at(i), 3) * pow(T.getY().at(i), 3)) * (T.getY().at(i) - T.getY().at(i - 1)) + I.at(i - 1));
    }
    for (int i = 0; i <= T.getX().size() - 1; i++) {
        S[i] = I.at(i) + R.at(i);
    }
    Trajektoria P(cas, S);
    T = P;
    return T;
}

int main()
{
    srand((unsigned)time(NULL));
    int  n = 1000;
    int d = 999;
    double T = 1000.;
    double krok = T / (double)n;
    std::vector<double> casy;
    casy.push_back(0);
    for (int i = 1; i <= n - 1; i++) {
        casy.push_back(casy[i - 1] + krok);
    }
    std::vector<Trajektoria> traj;
    for (int i = 0; i <= d; i++) {
        std::vector<double> B;
        Trajektoria TT;
        TT = generuj(casy, TT);
        traj.push_back(TT);
    }
    // ulozenie(traj, d, 0);

    std::vector<Trajektoria> I;
    for (int i = 0; i <= d; i++) {
        Trajektoria T = ito_1sposob(casy, traj[i]);
        I.push_back(T);
    }
    ulozenie(I, d, 1);

    std::vector<Trajektoria> II;
    for (int i = 0; i <= d; i++) {
        Trajektoria T = ito_2sposob(casy, traj[i]);
        II.push_back(T);
    }
    ulozenie(II, d, 2);
}