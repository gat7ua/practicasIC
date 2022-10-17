set terminal png
set output "tiemposCanny.png"

set xlabel "Threshold"
set ylabel "Sigma value"

set title "Evaluacón de una imagen .pmg con diversos valores"
splot "tiempos.txt" using 1:2:3 with lines title ""