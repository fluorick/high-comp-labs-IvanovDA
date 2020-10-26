#include <iostream>
#include <ctime>
#include <cmath>
#define N 100000

int main(int argc, char const *argv[])
{
	double a[N];
	double y,x;
	int i;
	
	unsigned t0 = clock();

		# pragma omp parallel for private (i, x, y) shared(a)
		for (i = 0; i < N; i++)
		{
			y = i*sin(i/N*3.14);
			x = i*cos(i/N*3.14);
			a[i] = y+x;
		}
		std::cout << clock() - t0 << std::endl;
	return 0;
}
