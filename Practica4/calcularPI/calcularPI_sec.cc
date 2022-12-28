// Algoritmo sacado de https://parzibyte.me/blog/2020/04/15/c-calculo-pi-serie-leibniz/ 
// para hacer calculos de tiempo
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <chrono>
#include <iostream>

using namespace std;

double calcularPi(double iteraciones) {
    auto start = std::chrono::high_resolution_clock::now();
    double numerador = 4;
    double denominador = 1; 
    double pi = 0;
    int x = 0;

    int operador = 1;
    for (x; x < iteraciones; x++) {
    pi += operador * (numerador / denominador);
    denominador += 2;
    operador *= -1;
    }
    return pi;
}

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    double pi = 0;
    
    for(int iteraciones = 1; iteraciones < 500000; iteraciones++){
        pi = calcularPi(iteraciones);
    }

    cout << "El valor de PI es: " << pi << endl;

    auto end = std::chrono::high_resolution_clock::now();
	auto tiempoTotalm = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	cout << "Tiempo de ejecución total (milisegundos) = " << tiempoTotalm.count() << endl;

    return 0;
}