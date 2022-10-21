// Soliman Alnaizy, UCFID: 3715450
// CAP 4453, Dr. Lobo, Fall 2018
// ========================================================
// ASSIGNMENT #1.2: C A N N Y   E D G E   D E T E C T I O N
// ========================================================


// Cabeceras
#include <iostream>
#include <vector>
#include <chrono>
#include <string.h>

#include "header_files/canny.hpp"
#include "header_files/global.hpp"
#include "header_files/HashMap.hpp"

using namespace std;

// Variables globales
char type[10];
string filename;
int height;
int width;
int intensity;
int hi;
int lo;
double sig;

int main(int argc, char **argv)
{
	bool activaBucle = false;
	int max_threshold = 1, max_value = 1;
	int iniT = 1, iniZ = 1;
	// Control de argumentos
	if (argc != 4)
	{
		cout << "Entrando en modo automático" << endl;
		cout << "A continuación se ejecturá el algoritmo con distintos valores para el estudio del tiempo" << endl;
		activaBucle = true;
		max_threshold = 15;
		max_value = 15;	
		iniZ = 5;
		iniT = 3;
	}
	// Apertura del fichero
	filename = string(argv[1]);
	string path = "./input_images/" + filename;
	ifstream infile(path, ios::binary);
	if (!infile.is_open())
	{
		cout << "File " << path << " not found in directory." << endl;
		return 0;
	}	

	cout << "Resultados de la ejecución" << endl;
	cout << "------------------------------------------------------------------------------" << endl;
	cout << "Threshold \t Sigma value \t Tiempo de ejecución" << endl;

	for(int z = iniZ; z <= max_threshold; z++){
		for(int t = iniT; t <= max_value; t++){
			
				// Empezamos el cronometro
				auto start = std::chrono::high_resolution_clock::now();
				// Ficheros de salida
				ofstream img1("./output_images/canny_mag.pgm", ios::binary);
				ofstream img2("./output_images/canny_peaks.pgm", ios::binary);		
				ofstream img3("./output_images/canny_final.pgm", ios::binary);

				// Si estamos en modo bucle, los valores cambiarán
				if(activaBucle == true){
					::hi = z;
					::lo = .35 * hi;
					::sig = t;
				}
				// Sino cogemos de los argumentos
				else{
					::hi = stoi(argv[2]);
					::lo = .35 * hi;
					::sig = stoi(argv[3]);
				}

				// Lectura de los datos de las imágenes
				infile >> ::type >> ::width >> ::height >> ::intensity;
				img1 << type << endl << width << " " << height << endl << intensity << endl;
				img2 << type << endl << width << " " << height << endl << intensity << endl;
				img3 << type << endl << width << " " << height << endl << intensity << endl;


				// Creación de matrices para almacenar las imágenes y sus máscaras en forma de matriz
				double **pic = new double*[height], **mag = new double*[height], **final = new double*[height];
				double **x = new double*[height], **y = new double*[height];

				for (int i = 0; i < height; i++)
				{
					pic[i] = new double[width];
					mag[i] = new double[width];
					final[i] = new double[width];
					x[i] = new double[width];
					y[i] = new double[width];
				}

				// Lectura de la matriz
				for (int i = 0; i < height; i++)
					for (int j = 0; j < width; j++)
						pic[i][j] = (int)infile.get();

				// Creación de la matriz de magnitud
				
				magnitude_matrix(pic, mag, x, y);
				
				

				// Guardar los picos en el vector
				HashMap *peaks = new HashMap();
				vector<Point*> v = peak_detection(mag, peaks, x, y);

				// Llamada a la función para 

				// Go through the vector and call the recursive function and each point. If the value
				// in the mag matrix is hi, then immediately accept it in final. If lo, then immediately
				// reject. If between lo and hi, then check if it's next to a hi pixel using recursion
				HashMap *h = new HashMap();
				int a, b;
				for (int i = 0; i < v.size(); i++)
				{
					a = v.at(i)->x;
					b = v.at(i)->y;

					if (mag[a][b] >= hi)
						final[a][b] = 255;
					else if (mag[a][b] < lo)
						final[a][b] = 0;
					else
						recursiveDT(mag, final, h, peaks, a, b, 0);
				}

				// ================================= IMAGE OUTPUT =================================
				// Outputting the 'mag' matrix to img1. It's very important to cast it to a char.
				// To make sure that the decimal doesn't produce any wonky results, cast to an int
				// ================================= IMAGE OUTPUT =================================
				for (int i = 0; i < height; i++)
					for (int j = 0; j < width; j++)
						img1 << (char)((int)mag[i][j]);

				// Outputting the points stored in the vector to img2
				int k = 0;
				for (int i = 0; i < v.size(); i++)
				{
					while(k++ != (v.at(i)->x * height + v.at(i)->y - 1))
						img2 << (char)(0);

					img2 << (char)(255);
				}


				// Output the 'final' matrix to img1
				for (int i = 0; i < height; i++)
					for (int j = 0; j < width; j++)
						img3 << (char)((int)final[i][j]);		

				auto end = std::chrono::high_resolution_clock::now();
				auto tiempoTotals = std::chrono::duration_cast<std::chrono::seconds>(end-start);
				auto tiempoTotalm = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

				// cout << "Tiempo en segundos " << tiempoTotals.count() << endl;
				// cout << "Tiempo total en milisegundos " << tiempoTotalm.count() << endl;
				auto tiempoTotalv = std::chrono::duration_cast<std::chrono::milliseconds>(end-start) - std::chrono::duration_cast<std::chrono::seconds>(end-start);

				// Para sacarlo bonito
				// cout << ::hi << "\t\t\t    " << ::sig << "\t\t\t    " << tiempoTotals.count() << "." << tiempoTotalv.count() << endl;
				// Para sacarlo al excel
				cout << ::hi << "\t " << ::sig << "\t " << tiempoTotals.count() << "," << tiempoTotalv.count() << endl;
			}
		}
	}
	cout << "Tamaño de la imagen en pixeles: " << ::height * ::width << endl;
	return 0;
}
