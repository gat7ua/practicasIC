#include <iostream>
#include <cstdlib>
#include <chrono>
#include <stdlib.h>
#include <mpi.h>

using namespace std;

const int N = 400;

void mult(int mat1[][N], int mat2[][N], int res[][N]){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            res[i][j] = 0;
            for (int k = 0; k < N; k++){
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

int main(){
    MPI_Init(NULL, NULL);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    auto start = std::chrono::high_resolution_clock::now();
    int mat1[N][N]; 
    int mat2[N][N];
    int mat3[N][N];  
    int mat4[N][N];
    int res1[N][N];
    int res2[N][N];
    int res3[N][N];

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            srand(time(NULL));
            mat1[i][j] = rand();  
            mat2[i][j] = rand();  
            mat3[i][j] = rand();  
            mat4[i][j] = rand(); 
        }
    }

    if (rank == 0) {
        MPI_Send(mat1, N*N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(mat2, N*N, MPI_INT, 1, 0, MPI_COMM_WORLD); 
    } else if (rank == 1) {
        int recv1[N][N], recv2[N][N];
        MPI_Recv(recv1, N*N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(recv2, N*N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        mult(recv1,recv2, res1);
        MPI_Send(res1, N*N, MPI_INT, 0, 0, MPI_COMM_WORLD); 
    }

    if (rank == 0) {
        MPI_Send(mat3, N*N, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send(mat4, N*N, MPI_INT, 2, 0, MPI_COMM_WORLD);
    } else if (rank == 2) {
        int recv1[N][N], recv2[N][N];
        MPI_Recv(recv1, N*N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(recv2, N*N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        mult(recv1, recv2, res2);
        MPI_Send(res2, N*N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        int recv1[N][N], recv2[N][N];
        MPI_Recv(recv1, N*N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(recv2, N*N, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        mult(recv1, recv2, res3);
    }


    MPI_Finalize();
    auto end = std::chrono::high_resolution_clock::now();
    auto tiempoTotalm = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Tiempo de ejecución total (milisegundos) = " << tiempoTotalm.count() << endl;

    return 0;
}

