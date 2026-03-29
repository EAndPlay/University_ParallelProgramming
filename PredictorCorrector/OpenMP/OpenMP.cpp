#include "omp.h"
#include <cmath>
#include <iostream>

using std::cout, std::endl;

void f(double x, double y1, double y2, double& f1, double& f2)
{
#pragma omp parallel sections
	{
#pragma omp section
		f1 = y2;
#pragma omp section
		f2 = -0.01 * y1 * exp(-x);
	}
}

int main()
{
	double f1, f2, y1 = 0.0, y2 = 0.5, x_start = 0.0, x_end = 7.0, x = x_start, h = 0.001,
		ff1, ff2, yy1, yy2, tn = omp_get_wtime(), tk;

#pragma omp parallel num_threads(2)
	{
		do
		{
#pragma omp barrier
			f(x, y1, y2, f1, f2);
#pragma omp section
				{
					yy1 = y1 + h * f1;
				}
#pragma omp section
				{
					yy2 = y2 + h * f2;
				}
#pragma omp barrier
			f(x + h, yy1, yy2, ff1, ff2);
#pragma omp section
				{
					y1 += 0.5 * (f1 + ff1) * h;
				}
#pragma omp section
				{
					y2 += 0.5 * (f2 + ff2) * h;
				}

			x += h;
		} while (x <= x_end);
	}
	tk = omp_get_wtime();
	cout << "y1 = " << y1 << endl << "y2 = " << y2 << endl << "time: " << tk - tn << endl;
}