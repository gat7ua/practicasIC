#include "header_files/canny.hpp"
#include "header_files/global.hpp"
#include "header_files/HashMap.hpp"

#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;
using namespace cv;

int height = 0;
int width = 0;
int hi;
int lo;
double sig;

bool debug = true;
int debVar = 1;

int main(int argc, char **argv)
{
	int max_value = 9, max_threshold = 90;
	int iniT = 9, iniZ = 90;

	// Comprueba que el número de argumentos es correcto
	if (argc < 4)
	{
		cout << "Entrando en modo automático" << endl;
		cout << "A continuación se ejecturá el algoritmo con distintos valores para el estudio del tiempo" << endl;
		max_threshold = 140;
		max_value = 20;	
		iniZ = 40;
		iniT = 3;
		//cout << "Proper syntax: ./a.out <input_filename> <high_threshold> <sigma_value>" << endl;
		//return 0;
	}
	else if (argc == 4)
	{
		iniZ = max_threshold = stoi(argv[2]);
		iniT = max_value = stoi(argv[3]);
	}

	string filename(argv[1]);
	string path = "./input_images/" + filename;

	Mat image = imread(path, IMREAD_GRAYSCALE);

	::width = image.cols;
	::height = image.rows;

	// Comprueba que la imagen se ha podido abrir
	if (image.empty()) 
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	//--------------- Codigo para mostrar la imagen -----------------------
	/*String windowName = filename; //Name of the window
	namedWindow(windowName); // Create a window
	imshow(windowName, image); // Show our image inside the created window.
	waitKey(0); // Wait for any keystroke in the window
	destroyWindow(windowName);*/

	cout << "Resultados de la ejecución" << endl;
	cout << "------------------------------------------------------------------------------" << endl;
	cout << "Threshold \t Sigma value \t Tiempo de ejecución" << endl;

	for (int z = iniZ; z <= max_threshold; z += 20) {
		for (int t = iniT; t <= max_value; t += 3) {

			auto start = std::chrono::high_resolution_clock::now();
			
			if (::debug) { //1
				::debVar = 1;
				cout << "P" << ::debVar << endl;
				::debVar++;
			}

			// Matrices de salida
			Mat im1 = Mat(image.rows, image.cols, (int) image.type()),
				im2 = Mat(image.rows, image.cols, (int) image.type()),
				im3 = Mat(image.rows, image.cols, (int) image.type());

			::width = image.cols;
			::height = image.rows;

			::hi = z;
			::lo = .35 * hi;
			::sig = t;


			if (::debug) { //2
				cout << "P" << ::debVar << endl;
				::debVar++;
			}

			// Estas matrices almacenan los valores de la imagen de entrada y la máscara
			double **pic = new double*[height], **mag = new double*[height], **final = new double*[height];
			double **x = new double*[height], **y = new double*[height];

			
			if (::debug) { //3
				cout << "P" << ::debVar << endl;
				::debVar++;
			}

			for (int i = 0; i < height; i++)
			{
				pic[i] = new double[width];
				mag[i] = new double[width];
				final[i] = new double[width];
				x[i] = new double[width];
				y[i] = new double[width];
			}


			if (::debug) { //4
				cout << "P" << ::debVar << endl;
				::debVar++;
			}

			// Almacena los datos en formato entero
			for (int i = 0; i < height; i++){
				for (int j = 0; j < width; j++) {
					pic[i][j] = (int) image.at<unsigned char>(i, j);
				}
			}

			if (::debug) { //5
				cout << "P" << ::debVar << endl;
				::debVar++;
			}

			// Crea la matriz magnitud
			magnitude_matrix(pic, mag, x, y);


			if (::debug) { //6
				cout << "P" << ::debVar << endl;
				::debVar++;
			}

			// Obtiene los picos y los almacena en un vector
			HashMap *peaks = new HashMap();
			vector<HPoint*> v = peak_detection(mag, peaks, x, y);

			if (::debug) { //7
				cout << "P" << ::debVar << endl;
				::debVar++;
			}

			// Recorre el vector y llama a la función recursiva para cada punto. Si el valor en mag
			// es = a hi, llega al final. Si el valor es = a lo, lo rechaza. Si está entre hi y lo,
			// comprueba si está al lado de un pixel = a hi usando recursividad.
			HashMap *h = new HashMap();
			int a, b;
			for (int i = 0; i < v.size(); i++)
			{
				a = v.at(i)->x;
				b = v.at(i)->y;
				
				cout << hi << ", " << lo << " (" << a << ", " << b << ") - " << mag[a][b] << endl;
				if (mag[a][b] >= hi) {
					cout << "OWO";
					final[a][b] = 255; }
				else if (mag[a][b] < lo) {
					cout << "AWA";
					final[a][b] = 0;}
				else { cout << "UWU";
					recursiveDT(mag, final, h, peaks, a, b, 0); }
			}

			if (::debug) { //8
				cout << "P" << ::debVar << endl;
				::debVar++;
			}

			// ================================= IMAGE OUTPUT =================================
			// Outputting the 'mag' matrix to img1. It's very important to cast it to a char.
			// To make sure that the decimal doesn't produce any wonky results, cast to an int
			// ================================= IMAGE OUTPUT =================================
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					im1.at<unsigned char>(i, j) = (unsigned char)((int)mag[i][j]);

			if (::debug) { //9
				cout << "P" << ::debVar << endl;
				::debVar++;
			}

			// Outputting the points stored in the vector to img2
			/*int k = 0;
			for (int i = 0; i < v.size(); i++)
			{
				int j = i / height;
				while(k++ != (v.at(i)->x * height + v.at(i)->y - 1))
					im2.at<unsigned char>(i % height, j) << (unsigned char)(0);
				im2.at<unsigned char>(i, j) = (unsigned char)(255);
			}*/

			if (::debug) { //10
				cout << "P" << ::debVar << endl;
				::debVar++;
			}

			// Output the 'final' matrix to img1
			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					im3.at<unsigned char>(i, j) = (unsigned char)((int)final[i][j]);		

			if (::debug) { //11
				cout << "P" << ::debVar << endl;
				::debVar++;
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto tiempoTotals = std::chrono::duration_cast<std::chrono::seconds>(end-start);
			auto tiempoTotalm = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

			// cout << "Tiempo en segundos " << tiempoTotals.count() << endl;
			// cout << "Tiempo total en milisegundos " << tiempoTotalm.count() << endl;
			auto tiempoTotalv = std::chrono::duration_cast<std::chrono::milliseconds>(end-start) - 
								std::chrono::duration_cast<std::chrono::seconds>(end-start);

			// Para sacarlo bonito
			cout << ::hi << "\t\t\t    " << ::sig << "\t\t\t    " << tiempoTotals.count() << "." << tiempoTotalv.count() << endl;
			// Para sacarlo al excel
			// cout << ::hi << "\t " << ::sig << "\t " << tiempoTotals.count() << "," << tiempoTotalv.count() << endl;
			// cout << "Threshold " << z << "\tSigma value " << t << "\tTiempo de ejecución " << tiempoTotals.count() << 
			//		"," << tiempoTotalv.count() << " segundos" << endl;



			//imwrite("./output_images/canny_mag.png", im1);
			//imwrite("./output_images/canny_peaks.png", im2);
			//imwrite("./output_images/canny_final.png", im3);

			delete[] pic; delete[] mag; delete[] final;
			delete[] x; delete[] y;
			delete h; delete peaks;
			v.clear();
		}
	}
	cout << "Tamaño de la imagen en pixeles: " << ::height * ::width << endl;

	return 0;
}
