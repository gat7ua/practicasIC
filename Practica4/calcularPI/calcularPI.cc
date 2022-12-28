#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <chrono>
#include <iostream>

using namespace std;
int main(int argc, char* argv[]){
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    long long n = 500000;
    double h = 1.0 / (double) n;
    double sum = 0.0;
    long long i;
    auto start = std::chrono::high_resolution_clock::now();

    for (i = rank; i < n; i += size)
    {
        double x = h * ((double) i + 0.5);
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = h * sum;

    double global_sum;
    MPI_Reduce(&pi, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0){
        cout << "\nEl valor de pi es: " << global_sum << endl;
        cout << "----------------------------------------------------------------------------" << endl;
    }

    MPI_Finalize();
	auto end = std::chrono::high_resolution_clock::now();
	auto tiempoTotalm = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    cout << "PID: " << getpt() << ", proceso: " << rank << " de " << size << ", procesador " << sched_getcpu() << endl;
	cout << "Tiempo de ejecución total (milisegundos) = " << tiempoTotalm.count() << endl;
    cout << "----------------------------------------------------------------------------" << endl;

    return 0;
}