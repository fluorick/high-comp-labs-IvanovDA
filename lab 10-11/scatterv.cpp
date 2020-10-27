#include <mpi.h>
#include <stdio.h>
#include <ctime>
#include <cmath>
#include <iostream>
#define SHARE 20000

int main(int *argc, char** argv)
{
    int numtasks, myID;
    int rbuf[SHARE];
    unsigned exec_t = 0;
    srand((unsigned)time(NULL));
    unsigned t0 = clock();
    
    MPI_Init(argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    int* sendbuf = new int[numtasks * SHARE];
        if (myID == 0) {
            for (int j = 0; j < numtasks * SHARE; j++) {
                sendbuf[j] = rand() % 1000;
            }
        }
        MPI_Scatter(sendbuf, SHARE, MPI_INT, rbuf, SHARE, MPI_INT, 0, MPI_COMM_WORLD);
        unsigned my_exec_t = clock() - t0;
        if (my_exec_t > exec_t) {
            exec_t = my_exec_t;
        }
        if (myID == 0) {
            std::cout << "SCATTERV Execution time: " << exec_t << std::endl;
        }
        MPI_Finalize();
}