#include <mpi.h>
#include <stdio.h>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#define N1 4
#define N2 4
#define N3 4
#define THREADS 2

void prepare_data(unsigned procAmount, unsigned n1, unsigned n2, unsigned n3) {
    if ((procAmount >= 2) && (n1 >= procAmount) && (n2 >= procAmount) && (n3 >= procAmount) && (n1 % procAmount == 0) && (n2 % procAmount == 0) && (n3 % procAmount == 0)) {
        std::ofstream myfile;
        srand(time(NULL));
        myfile.open("source_data.txt");
        for (unsigned i = 0; i < n1; i++) {
            for (unsigned j = 0; j < n2; j++) {
                myfile << (rand() % 20) - 10 << " ";
            }
            myfile << "\n";
        }
        myfile << "\n";
        for (unsigned i = 0; i < n2; i++) {
            for (unsigned j = 0; j < n3; j++) {
                myfile << (rand() % 10) - 5 << " ";
            }
            myfile << "\n";
        }
        myfile.close();
    }
    else {
        std::cout << "(n1%p == 0)&&(n2%p == 0)&&(n3%p == 0)&&(n1 > p)&&(n2 > p)&&(n3 > p) must be True.";
        exit(0);
    }
}

int* matrixMultiply(const int* m1, size_t m1Rows, size_t m1Cols,
    const int* m2, size_t m2Rows, size_t m2Cols) {
    if (m1Cols != m2Rows) {
        return nullptr;
    }
    int* res = new int[m1Rows * m2Cols]{ 0 };
    for (size_t i = 0; i < m1Rows; ++i) {
        for (size_t j = 0; j < m2Cols; ++j) {
            for (size_t k = 0; k < m1Cols; ++k) {
                res[i * m2Cols + j] += m1[i * m1Cols + k] * m2[k * m2Cols + j];
            }
        }
    }
    return res;
}

int main(int *argc, char** argv)
{
    int numtasks, myID;
    MPI_Status Stat;
  
    MPI_Init(argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    int myShare[N2 * (N1 / THREADS)];

    int* A = new int[N1 * N2];
    int* B = new int[N2 * N3];
  
        if (myID == 0) {
            prepare_data(numtasks, N1, N2, N3);
            
            std::ifstream inputFile("source_data.txt");
            if (inputFile) {
                int value;
                for (int i = 0; i < N1 * N2; i++) {
                    inputFile >> value;
                    A[i] = value;
                }
                for (int i = 0; i < N2 * N3; i++) {
                    inputFile >> value;
                    B[i] = value;
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Scatter(A, N2 * (int)(N1 / numtasks), MPI_INT, myShare, N2 * (int)(N1 / numtasks), MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(B, N2 * N3, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

        int* myResults = matrixMultiply(myShare, (int)(N1 / numtasks), N2, B, N2, N3);
        if (myID != 0) {
            MPI_Send(myResults, (int)(N1 / numtasks) * N3, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
        
        MPI_Barrier(MPI_COMM_WORLD);
        
        if (myID == 0) {
            std::ofstream myfile;
            myfile.open("result.txt");
            for (int i = 0; i < (int)(N1 / numtasks) * N3; i++) {
                myfile << myResults[i] << " ";
                if (((i + 1) % N3) == 0) {
                    myfile << "\n";
                }
            }
            for (int i = 1; i < numtasks; i++) {
                int* resultsGOT = new int[(int)(N1 / numtasks) * N3];
                MPI_Recv(resultsGOT, (int)(N1 / numtasks) * N3, MPI_INT, i, 1, MPI_COMM_WORLD, &Stat);
                for (int j = 0; j < (int)(N1 / numtasks) * N3; j++) {
                    myfile << resultsGOT[j] << " ";
                    if (((j + 1) % N3) == 0) {
                        myfile << "\n";
                    }
                }
            }
            myfile.close();
            
        }    
        MPI_Finalize();    
}