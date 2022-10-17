CC = g++
CFLAGS = -g -Wall

OPENCV = `pkg-config --cflags --libs opencv4`
LIBS = $(OPENCV)

canny: main.o canny.o HashMap.o
	$(CC) $(CFLAGS) -o canny main.o canny.o HashMap.o $(LIBS)
main.o: main.cpp
	$(CC) -c main.cpp $(LIBS)
canny.o: canny.cpp
	$(CC) -c canny.cpp
HashMap.o: HashMap.cpp
	$(CC) -c HashMap.cpp
clean:
	rm -f *.o canny output_images/*.pgm
