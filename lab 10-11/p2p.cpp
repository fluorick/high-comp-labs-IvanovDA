#include <mpi.h>
#include <stdio.h>
#include <ctime>
#include <cmath>
#include <iostream>
#define ARRSIZE 1000
#define SHARE 200

int main(int *argc, char** argv)
{
    int numtasks, myID, tag = 1;
    MPI_Status Stat;
    unsigned exec_t = 0;
    int NUM = 1;

    int info[ARRSIZE];
    srand((unsigned)time(NULL));

    unsigned t0 = clock();
    MPI_Init(argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myID);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   

        if (myID == 0) {
            for (int j = 0; j < ARRSIZE; j++) {
                info[j] = rand() % 1000; 
            }
            for (int j = 1; j < numtasks; j++) {
                int sub_info[SHARE];
                int ind = 0;
                for (int z = SHARE * (j - 1); z < SHARE * j; z++) {
                    sub_info[ind] = info[z];
                    ind++;
                }
                MPI_Send(&sub_info, SHARE, MPI_INT, j, tag, MPI_COMM_WORLD);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if (myID != 0) {
            int sub_info[SHARE];           
            MPI_Recv(&sub_info, SHARE, MPI_INT, 0, tag, MPI_COMM_WORLD, &Stat);  
        }

        unsigned my_exec_t = clock() - t0;
        if (my_exec_t > exec_t) {
            exec_t = my_exec_t;
        }
        
        if (myID == 0) {
            std::cout << "P2P Execution time: " << exec_t << std::endl;
        }
        MPI_Finalize();
        
}