#include <iostream>
#include <vector>
#include <chrono>
#include <string.h>

#include "header_files/canny.hpp"
#include "header_files/global.hpp"
#include "header_files/HashMap.hpp"

using namespace std;

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
	int max_value = 9, max_threshold = 90;
	int iniT = 9, iniZ = 90;

	// Comprueba que el número de argumentos es correcto
	// Control de argumentos
	if (argc < 4)
	{
		cout << "Entrando en modo automático" << endl;
		cout << "A continuación se ejecturá el algoritmo con distintos valores para el estudio del tiempo" << endl;
		max_threshold = 140;
		max_value = 20;
		iniZ = 40;
		iniT = 3;
	}
	else if (argc == 4)
	{
		iniZ = max_threshold = stoi(argv[2]);
		iniT = max_value = stoi(argv[3]);
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

	for(int z = iniZ; z <= max_threshold; z+=20){
		for(int t = iniT; t <= max_value; t+=3){
			
			// Empezamos el cronometro
			auto start = std::chrono::high_resolution_clock::now();
			// Ficheros de salida
			ofstream img1("./output_images/canny_mag.pgm", ios::binary);
			ofstream img2("./output_images/canny_peaks.pgm", ios::binary);		
			ofstream img3("./output_images/canny_final.pgm", ios::binary);

			::hi = z;
			::lo = .35 * hi;
			::sig = t;

			// Guardando información de las cabeceras y copiandola en las imágenes de salida
			infile >> ::type >> ::width >> ::height >> ::intensity;
			img1 << type << endl
				 << width << " " << height << endl
				 << intensity << endl;
			img2 << type << endl
				 << width << " " << height << endl
				 << intensity << endl;
			img3 << type << endl
				 << width << " " << height << endl
				 << intensity << endl;

			// Estas matrices almacenan los valores de la imagen de entrada y la máscara
			double **pic = new double *[height], **mag = new double *[height], ** final = new double *[height];
			double **x = new double *[height], **y = new double *[height];

			for (int i = 0; i < height; i++)
			{
				pic[i] = new double[width];
				mag[i] = new double[width];
				final[i] = new double[width];
				x[i] = new double[width];
				y[i] = new double[width];
			}

			// Almacena los datos en formato entero
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					pic[i][j] = (int)infile.get();

			// Crea la matriz magnitud
			magnitude_matrix(pic, mag, x, y);
			
			// Obtiene los picos y los almacena en un vector
			HashMap *peaks = new HashMap();
			vector<Point *> v = peak_detection(mag, peaks, x, y);

			// Recorre el vector y llama a la función recursiva para cada punto. Si el valor en mag
			// es = a hi, llega al final. Si el valor es = a lo, lo rechaza. Si está entre hi y lo,
			// comprueba si está al lado de un pixel = a hi usando recursividad.
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

			// ================================= SALIDA DE IMAGENES =================================
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					img1 << (char)((int)mag[i][j]);

			int k = 0;
			for (int i = 0; i < v.size(); i++)
			{
				while (k++ != (v.at(i)->x * height + v.at(i)->y - 1))
					img2 << (char)(0);

				img2 << (char)(255);
			}

			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					img3 << (char)((int) final[i][j]);

			auto end = std::chrono::high_resolution_clock::now();
			auto tiempoTotals = std::chrono::duration_cast<std::chrono::seconds>(end - start);
			auto tiempoTotalm = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			// cout << "Tiempo en segundos " << tiempoTotals.count() << endl;
			// cout << "Tiempo total en milisegundos " << tiempoTotalm.count() << endl;
			auto tiempoTotalv = std::chrono::duration_cast<std::chrono::milliseconds>(end - start) - std::chrono::duration_cast<std::chrono::seconds>(end - start);

			// Para sacarlo bonito
			// cout << ::hi << "\t\t\t    " << ::sig << "\t\t\t    " << tiempoTotals.count() << "." << tiempoTotalv.count() << endl;
			// Para sacarlo al excel
			cout << ::hi << "\t " << ::sig << "\t " << tiempoTotals.count() << "," << tiempoTotalv.count() << endl;
			// cout << "Threshold " << z << "\tSigma value " << t << "\tTiempo de ejecución " << tiempoTotals.count() << "," << tiempoTotalv.count() << " segundos" << endl;
		}
	}
	cout << "Tamaño de la imagen en pixeles: " << ::height * ::width << endl;
	return 0;
}
