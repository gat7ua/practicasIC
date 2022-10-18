#include "header_files/canny.hpp"
#include "header_files/global.hpp"
#include "header_files/HashMap.hpp"

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

char type[10];
int height;
int width;
int intensity;
int hi;
int lo;
double sig;

int main(int argc, char **argv)
{
	// Comprueba que el número de argumentos es correcto
	if (argc != 4)
	{
		cout << "Proper syntax: ./a.out <input_filename> <high_threshold> <sigma_value>" << endl;
		return 0;
	}

	string filename(argv[1]);
	string path = "./input_images/" + filename;

	Mat image = imread(path, IMREAD_GRAYSCALE);

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

	ifstream infile(path, ios::binary);
	if (!infile.is_open())
	{
		cout << "File " << path << " not found in directory." << endl;
		return 0;
	}	

	// Matrices de salida
	Mat im1 = Mat(image.rows, image.cols, (int) image.type()),
		im2 = Mat(image.rows, image.cols, (int) image.type()),
		im3 = Mat(image.rows, image.cols, (int) image.type());

	::hi = stoi(argv[2]);
	::lo = .35 * hi;
	::sig = stoi(argv[3]);

	::width = image.cols;
	::height = image.rows;

	// Estas matrices almacenan los valores de la imagen de entrada y la máscara
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

	// Almacena los datos en formato entero
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++) {
			pic[i][j] = (int) image.at<unsigned char>(i, j);
		}
	}

	// Crea la matriz magnitud
	magnitude_matrix(pic, mag, x, y);

	// Obtiene los picos y los almacena en un vector
	HashMap *peaks = new HashMap();
	vector<HPoint*> v = peak_detection(mag, peaks, x, y);

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
			im1.at<unsigned char>(i, j) = (unsigned char)((int)mag[i][j]);

	// Outputting the points stored in the vector to img2
	int k = 0;
	for (int i = 0; i < v.size(); i++)
	{
		int j = i / height;
		while(k++ != (v.at(i)->x * height + v.at(i)->y - 1))
			im2.at<unsigned char>(i - height * j, j) << (unsigned char)(0);

		im2.at<unsigned char>(i, j) << (unsigned char)(255);
	}

	// Output the 'final' matrix to img1
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			im3.at<unsigned char>(i, j) = (unsigned char)((int)final[i][j]);		

	imwrite("./output_images/canny_mag.png", im1);
	imwrite("./output_images/canny_peaks.png", im2);
	imwrite("./output_images/canny_final.png", im3);

	return 0;
}
