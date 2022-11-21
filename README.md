# practicasIC

## Para compilar el código neuroevolutivo:

mkdir build && cd build
cmake ..
./evolution follow -s

## Hay que paralelizar
En genetic.cc -> método nextGeneration
En simulation.cc -> método update (for)
