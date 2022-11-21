# practicasIC

## Para compilar el código neuroevolutivo:

mkdir build && cd build <br>
cmake .. <br>
./evolution follow -s <br>

## Hay que paralelizar
En genetic.cc -> método nextGeneration
En simulation.cc -> método update (for)
