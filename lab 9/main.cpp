#include <mpi.h>
#include <stdio.h>
#include <ctime>
#include <iostream>

int main(int *argc, char** argv)
{
    int numtasks, myID, nameLen, rc, tag = 1;
    char processorName[MPI_MAX_PROCESSOR_NAME];
    MPI_Status Stat;

    MPI_Init(argc, &argv);
  
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Get_processor_name(processorName, &nameLen);

    

    if (myID != 0) {
        srand((unsigned)time(NULL) + myID * numtasks + nameLen);
        int my_balance = (rand() % 200 - 100) * 100;
        rc = MPI_Send(&my_balance, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);

        if (my_balance < 0) {
            printf("Hey, I'm from the department #%d, this month sucks: %d\n", myID, my_balance);
        }
        else if (my_balance > 0) {
            printf("Hey, I'm from the department #%d, we're good this month: +%d\n", myID, my_balance);
        }
        else {
            printf("Hey, I'm from the department #%d, this month we're zero. LOL.", myID);
        }
    }
    else {
        int total_month_stats = 0;

        for (int i = 1; i < 11; i++) {
            int info_got;
            rc = MPI_Recv(&info_got, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
            total_month_stats += info_got;
        }
        printf("\n");
        if (total_month_stats > 0) {
            printf("Well, I've summed everything up, we're doing great: our month recap is +%d", total_month_stats);
        }
        else if (total_month_stats < 0) {
            printf("Nah, I've summed everything up, we're screwed: our month recap is %d", total_month_stats);
        }
        else {
            printf("LOL, guys, we're all zero this month!");
        }
    }
    MPI_Finalize();
}