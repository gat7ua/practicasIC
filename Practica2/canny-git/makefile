canny: main.cpp HashMap.cpp canny.cpp
	g++ main.cpp canny.cpp HashMap.cpp -o main
main.o: main.cpp
	g++ -c main.cpp
canny.o: canny.cpp
	g++ -c canny.cpp
HashMap.o: HashMap.cpp
	g++ -c HashMap.cpp
clean:
	rm -f *.o *.out output_images/*.pgm
tiempos: main
	./main > tiempos
run:
	./main bird.pgm 10 10