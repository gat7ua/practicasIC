#include <iostream>
#include <cstdlib>
#include <chrono>
#include <stdlib.h>
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
    auto start = std::chrono::high_resolution_clock::now();
    int mat1[N][N]; 
    int mat2[N][N];
    int mat3[N][N];  
    int mat4[N][N];
    int res1[N][N];
    int res2[N][N];
    int res3[N][N];

    // Inicializar matrices
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            srand(time(NULL));
            mat1[i][j] = rand();  
            mat2[i][j] = rand();  
            mat3[i][j] = rand();  
            mat4[i][j] = rand(); 
        }
    }

    mult(mat1, mat2, res1);
    mult(mat3, mat4, res2);
    mult(res1, res2, res3);

    // for (int i = 0; i < N; i++){
    //     for (int j = 0; j < N; j++){
    //         cout << res3[i][j] << " ";
    //     }

    //     cout << endl;
    // }

    auto end = std::chrono::high_resolution_clock::now();
	auto tiempoTotalm = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Tiempo de ejecución total (milisegundos) = " << tiempoTotalm.count() << endl;

    return 0;
}
