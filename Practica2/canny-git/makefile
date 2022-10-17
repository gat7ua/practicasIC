canny: main.o canny.o HashMap.o
	g++ -o canny.out main.o canny.o HashMap.o
main.o: main.cpp
	g++ -c main.cpp
canny.o: canny.cpp
	g++ -c canny.cpp
HashMap.o: HashMap.cpp
	g++ -c HashMap.cpp
clean:
	rm -f *.o *.out output_images/*.pgm
run:
	./canny.out chess.pgm 100 1