## Para ejecutar todo haz make tiempo

cTime = ./tiempoCompilacion
eTime = ./tiempoEjecucion
mFolder = ./mains

canny: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -lstdc++ -o ${mFolder}/mainNoOpt
canny1: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -lstdc++ -O1 -o ${mFolder}/mainO1
canny2: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -lstdc++ -O2 -o ${mFolder}/mainO2
canny3: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -lstdc++ -O3 -o ${mFolder}/mainO3
cannys: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -lstdc++ -Os -o ${mFolder}/mainOs
cannyz: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -lstdc++ -OZ -o ${mFolder}/mainOz
############################################################
main.o: main.cpp
	g++ -c main.cpp
canny.o: canny.cpp
	g++ -c canny.cpp
HashMap.o: HashMap.cpp
	g++ -c HashMap.cpp
############################################################
clean:
	rm -f *.o *.out output_images/*.pgm *.png *.txt tiempoEjecucion/*.txt tiempoCompilacion/*.txt mains/*
############################################################
compilationTimeNoOpt: ${mFolder}/mainNoOpt
	time ${mFolder}/mainNoOpt > ${cTime}/compilacion_no_opt.txt
compilationTimeO1: ${mFolder}/mainO1
	time ${mFolder}/mainO1 > ${cTime}/tiempoCompilacion/compilacion_O1.txt
compilationTimeO2: ${mFolder}/mainO2
	time ${mFolder}/mainO2 > ${cTime}/compilacion_O2.txt
compilationTimeO3: ${mFolder}/mainO3
	time ${mFolder}/mainO3 > ${cTime}/compilacion_O3.txt
compilationTimeOs: ${mFolder}/mainOs
	time /mainOs > ${cTime}/compilacion_Os.txt
compilationTimeOz: ${mFolder}/mainOz
	time ${mFolder}/mainOz > ${cTime}/compilacion_Oz.txt
compilationTime: compilationTimeNoOpt compilationTimeO1 compilationTimeO2 compilationTimeO3 compilationTimeOs compilationTimeOz
############################################################
tiempoNoOpt: main.o canny.o HashMap.o canny mainNoOpt
	${mFolder}/mainNoOpt bird.pgm > ${eTime}/tiempoNoOpt.txt
tiempoO1: main.o canny.o HashMap.o canny1 mainO1
	${mFolder}/mainO1 bird.pgm > ${eTime}/tiempoO1.txt
tiempoO2: main.o canny.o HashMap.o canny2 mainO2
	${mFolder}/mainO2 bird.pgm > ${eTime}/tiempoO2.txt
tiempoO3: main.o canny.o HashMap.o canny3 mainO3
	${mFolder}/mainO3 bird.pgm > ${eTime}/tiempoO3.txt
tiempoOs: main.o canny.o HashMap.o cannys mainOs
	${mFolder}/mainOs bird.pgm > ${eTime}/tiempoOs.txt
tiempoOz: main.o canny.o HashMap.o cannyz mainOz
	${mFolder}/mainOz bird.pgm > ${eTime}/tiempoOz.txt
tiempo: clean tiempoNoOpt tiempoO1 tiempoO2 tiempoO3 tiempoOs compilationTime
############################################################
graphs: tiempos.txt tiempos.gnuplot
	gnuplot tiempos.gnuplot
############################################################
rendimiento: graphs tiempos tiempos.txt
############################################################
run:
	./main bird.pgm 10 10