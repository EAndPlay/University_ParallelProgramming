#include <mpi.h>
#include <cmath>
#include <iostream>

using std::cout, std::endl;

double f1(double x, double y1, double y2)
{
	return y2;
}

double f2(double x, double y1, double y2)
{
	return -0.01 * y1 * exp(-x);
}

int main(int argc, char** argv)
{
	int rank, size;
	if (MPI_Init(&argc, &argv) != MPI_SUCCESS) return 1;
	if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS)
	{
		MPI_Finalize();
		return 2;
	}
	if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS)
	{
		MPI_Finalize();
		return 3;
	}

	double(*func)(double, double, double) = rank ? f2 : f1;

	double f, ff, y[] = { 0.0, 0.5 }, yy[] = { 0.0, 0.0 }, &_y = y[rank], &_yy = yy[rank], x_start = 0.0, x_end = 7.0, x = x_start, h = 0.001, tn = MPI_Wtime(), tk;

	do
	{
		f = func(x, *y, y[1]);
		_yy = _y + h * f;
		MPI_Allgather(&yy[rank], 1, MPI_DOUBLE, yy, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		ff = func(x + h, *yy, yy[1]);
		_y += 0.5 * (f + ff) * h;
		MPI_Allgather(&_y, 1, MPI_DOUBLE, y, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		x += h;
	} while (x <= x_end);

	tk = MPI_Wtime();
	if (!rank)
	{
		cout << "y1 = " << y[0] << endl << "y2 = " << y[1] << endl << "time: " << tk - tn << endl;
	}
	MPI_Finalize();
	return 0;
}

// mpiexec -n 2 .\MapReduce.exe