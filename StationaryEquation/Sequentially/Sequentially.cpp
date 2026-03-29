#include <iostream>
#include <fstream>
#include <omp.h>
#include <cmath>

using std::cout, std::endl;

double Uab(double x, double y) { return 30 * y; }
double Ubc(double x, double y) { return 30 * (100 - x); }
double Ucd(double x, double y) { return 0; }
double Uad(double x, double y) { return 0; }

int main()
{
    const double SQUARE_MAX = 100.0, h = 0.1, SQUARE_MIN = 0.0;
    const int lmax = 10000;

    int n = (int)(SQUARE_MAX / h) + 1, m = (int)(SQUARE_MAX / h) + 1, idx, i, j;

    double* u = new double[n * m], * unew = new double[n * m], tn = omp_get_wtime(), tk;

    for (i = 0; i < n; i++)
    {
        double x = SQUARE_MIN + i * h;
        for (j = 0; j < m; j++)
        {
            double y = SQUARE_MIN + j * h;
            idx = i * m + j;

            if (i == 0)
            {
                u[idx] = Uab(x, y);
                unew[idx] = Uab(x, y);
            }
            else if (i == n - 1)
            {
                u[idx] = Ucd(x, y);
                unew[idx] = Ucd(x, y);
            }
            else if (j == 0)
            {
                u[idx] = Uad(x, y);
                unew[idx] = Uad(x, y);
            }
            else if (j == m - 1)
            {
                u[idx] = Ubc(x, y);
                unew[idx] = Ubc(x, y);
            }
            else
            {
                u[idx] = 0.0;
                unew[idx] = 0.0;
            }
        }
    }

    for (int l = 0; l < lmax; l++)
    {
        for (i = 1; i < n - 1; i++)
        {
            for (j = 1; j < m - 1; j++)
            {
                idx = i * m + j;
                double new_val = 0.25 * (u[idx - m] + u[idx + m] + u[idx - 1] + u[idx + 1]);
                unew[idx] = new_val;
            }
        }

        for (i = 1; i < n - 1; i++)
        {
            for (j = 1; j < m - 1; j++)
            {
                idx = i * m + j;
                u[idx] = unew[idx];
            }
        }
    }

    tk = omp_get_wtime();

    std::ofstream file("results.txt");
    for (i = 1; i < 50; i++)
    {
        for (j = 1; j < 50; j++)
        {
            idx = i * m + j;
            file << u[idx] << ' ';
        }
        file << '\n';
    }

    cout << "Time: " << tk - tn << " seconds" << endl;

    delete[] u;
    delete[] unew;

    return 0;
}