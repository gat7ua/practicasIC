## Para ejecutar todo haz make tiempo

cTime = ./tiempoCompilacion
eTime = ./tiempoEjecucion
mFolder = ./mains

# Tipos de compilación
############################################################
canny: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -lstdc++ -o mainNoOpt
canny1: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -lstdc++ -O1 -march=corei7 -o mainO1
canny2: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -lstdc++ -O2 -march=corei7 -o mainO2
canny3: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -lstdc++ -O3 -march=corei7 -o mainO3
cannyf: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -lstdc++ -Ofast -march=corei7 -o mainFast

# Generar los .o
############################################################
main.o: main.cpp
	g++ -c main.cpp
canny.o: canny.cpp
	g++ -c canny.cpp
HashMap.o: HashMap.cpp
	g++ -c HashMap.cpp

# Limpiar
############################################################
clean:
	rm -f *.o *.out output_images/*.pgm *.png *.txt tiempoEjecucion/*.txt tiempoCompilacion/*.txt mains/*

# Obtención de los tiempo de compilación
############################################################
# compilationTimeNoOpt: tiempoNoOpt mainNoOpt
# 	time mainNoOpt
# compilationTimeO1: mainO1
# 	time mainO1 > ${cTime}/compilacion_O1.txt
# compilationTimeO2: ${mFolder}/mainO2
# 	time ${mFolder}/mainO2 > ${cTime}/compilacion_O2.txt
# compilationTimeO3: ${mFolder}/mainO3
# 	time ${mFolder}/mainO3 > ${cTime}/compilacion_O3.txt
# compilationTimeFast: ${mFolder}/mainFast
# 	time ${mFolder}/mainFast > ${cTime}/compilacion_Fast.txt
# compilationTime: compilationTimeNoOpt compilationTimeO1 compilationTimeO2 compilationTimeO3 compilationTimeFast

# Obtención de los tiempos de ejecución
############################################################
tiempoNoOpt: main.o canny.o HashMap.o canny mainNoOpt
	./mainNoOpt bird.pgm > ${eTime}/tiempoNoOpt.txt
tiempoO1: main.o canny.o HashMap.o canny1 ${mFolder}/mainO1
	${mFolder}/mainO1 bird.pgm > ${eTime}/tiempoO1.txt
tiempoO2: main.o canny.o HashMap.o canny2 ${mFolder}/mainO2
	${mFolder}/mainO2 bird.pgm > ${eTime}/tiempoO2.txt
tiempoO3: main.o canny.o HashMap.o canny3 ${mFolder}/mainO3
	${mFolder}/mainO3 bird.pgm > ${eTime}/tiempoO3.txt
tiempoFast: main.o canny.o HashMap.o cannyf ${mFolder}/mainFast
	${mFolder}/mainFast bird.pgm > ${eTime}/tiempoFast.txt
tiempo: clean tiempoNoOpt tiempoO1 tiempoO2 tiempoO3 tiempoFast
############################################################
graphs: tiempos.txt tiempos.gnuplot
	gnuplot tiempos.gnuplot
############################################################
rendimiento: graphs tiempos tiempos.txt
############################################################
run:
	./main bird.pgm 10 10