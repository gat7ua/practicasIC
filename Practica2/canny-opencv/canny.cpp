#include "header_files/canny.hpp"
#include "header_files/global.hpp"

using namespace std;

// =================================== Magnitude Image ==================================
// Empieza llenando los valores de la máscara usando la 1a derivada Gaussiana. Después,
// hace un escaneo convolucional de la matriz de la imagen del input. Esto nos dará las
// matrices Δy y Δx. Finalmente, se coge la raíz de ña suma de los cuadrados de Δy^2 y
// Δx^2 para enconctrar la magitud.
// ======================================================================================
void magnitude_matrix(double **pic, double **mag, double **x, double **y)
{
	int dim = 6 * sig + 1, cent = dim / 2;
	double maskx[dim][dim], masky[dim][dim];

	// Usa la fórmula de la 1a derivada de Gauss para rellenar los valores de la máscara
	for (int p = -cent; p <= cent; p++)
	{	
		for (int q = -cent; q <= cent; q++)
		{
			maskx[p+cent][q+cent] = q * exp(-1 * ((p * p + q * q) / (2 * sig * sig)));
			masky[p+cent][q+cent] = p * exp(-1 * ((p * p + q * q) / (2 * sig * sig)));
		}
	}

	// Escaneo convolucional
	double sumx, sumy;
	for (int i = 0; i < height; i++)
	{ 
		for (int j = 0; j < width; j++)
		{
			sumx = 0;
			sumy = 0;

			// Esta es la convolución
			for (int p = -cent; p <= cent; p++)
			{
				for (int q = -cent; q <= cent; q++)
				{
					if ((i+p) < 0 || (j+q) < 0 || (i+p) >= height || (j+q) >= width)
						continue;
					
					sumx += pic[i+p][j+q] * maskx[p+cent][q+cent];
					sumy += pic[i+p][j+q] * masky[p+cent][q+cent];
				}
			}
			
			// Almacena el resultado de la convolución en la matriz respectiva
			x[i][j] = sumx;
			y[i][j] = sumy;
		}
	}

	// Encuantra la magitud y el valor máximo, entonces los almacena en la matriz 'mag'
	double mags;
	double maxVal = 0;
	for (int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			mags = sqrt((x[i][j] * x[i][j]) + (y[i][j] * y[i][j]));

			if (mags > maxVal)
				maxVal = mags;

			mag[i][j] = mags;
		}
	}

	// Se debe asegurar que los valores sean entre 0 y 255
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			mag[i][j] = mag[i][j] / maxVal * 255;

	return;
}

// ================================ Peaks Detection ================================
// La fórmula de la pendiente es Δy/Δx. Teniendo Δy y Δx del escaneo convolucional
// arriba, podemos obtener la pendiente dividiendo las dos. Guardaremos los puntos que
// son más grandes que sus vecinos en la dirección de la pendiente en un vector.
// Podemos calcular la dirección de la pendiente con tan(x). Además guardaremos los 
// picos en un HashMap para O(1) buscar más adelante en la funcioen recursiveDT.
// =================================================================================
vector<HPoint*> peak_detection(double **mag, HashMap *h, double **x, double **y)
{
	double slope = 0;
	vector<HPoint*> v;
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			// Para no dividir entre 0
			if (x[i][j] == 0)
				x[i][j] = 0.0001;

			slope = y[i][j] / x[i][j];


			// Sólo buscamos los picos. Si estamos en un pico almacenamos 255
			if (slope <= tan(22.5) && slope > tan(-22.5))
			{
				if (mag[i][j] > mag[i][j-1] && mag[i][j] > mag[i][j+1])
				{
					v.push_back(new HPoint(i, j));
					h->insert(i, j);
				}
			}
			else if (slope <= tan(67.5) && slope > tan(22.5))
			{
				if (mag[i][j] > mag[i-1][j-1] && mag[i][j] > mag[i+1][j+1])
				{
					v.push_back(new HPoint(i, j));
					h->insert(i, j);
				}
			}
			else if (slope <= tan(-22.5) && slope > tan(-67.5))
			{
				if (mag[i][j] > mag[i+1][j-1] && mag[i][j] > mag[i-1][j+1])
				{
					v.push_back(new HPoint(i, j));
					h->insert(i, j);
				}
			}
			else
			{
				if (mag[i][j] > mag[i-1][j] && mag[i][j] > mag[i+1][j])
				{
					v.push_back(new HPoint(i, j));
					h->insert(i, j);
				}
			}
		}
	}

	return v;
}

// ======================== Hysteresis & Double Thresholding ========================
// Los puntos que recibe la función vienen de los picos del vector. Empezamos buscando
// alrededor del pixel actual un pixel que nos lleve al 'final'. Si lo encontramos,
// recursivamente buscaremos la serie de píxeles que están en el medio rango y los 
// cambiaremos a ON en el final. Paramos tan pronto como todos los píxeles estén o 
// bien ya procesador o cuando sean menores que el umbral 'lo'.
// ==================================================================================
void recursiveDT(double **mag, double **final, HashMap *h, HashMap *peaks, int a, int b, int flag)
{
	
	cout << "e0";
	// Si el valor del píxel es < lo, fuera de los límites, o en el punto en que visitamos anteriormente,
	// salir de la función.
	if (a < 0 || b < 0 || a >= height || b >= width || mag[a][b] < lo)
		return;
	cout << "e1";
	if (h->contains(a, b))
		return;
	
	cout << "e2";
	// Inserta el píxel actual para saber que lo hemos visitado.
	h->insert(a, b);

	cout << "e3";
	// Si flag = 0, quiere decir que es el primer píxel de la serie que hemos visto.
	// Vamos a mirar un píxel en "final" que este puesto en ON. Si lo encontramos,
	// comprobamos el flag y salimos de los bucles.
	if (!flag)
	{
		for (int p = -1; p <= 1; p++)
		{
			for (int q = -1; q <= 1; q++)
			{
				if (final[a+p][b+q] == 255)
				{
					final[a][b] = 255;
					flag = 1;
					break;
				}
			}

			if (flag)
				break;
		}
	}
	
	// Si flag = 1, quiere decir que hemos encontrado un píxel que está en final
	// todo lo que tenemos que hacer es buscar píxeles que estén en mitad del rango.
	// Además, estar seguros de que está en los picos para empezar.
	if (flag)
	{
		for (int p = -1; p <= 1; p++)
		{
			for (int q = -1; q <= 1; q++)
			{
				if (mag[a+p][b+q] >= lo && peaks->contains(a+p, b+q))
				{
					recursiveDT(mag, final, h, peaks, a+p, b+q, 1);
					final[a][b] = 255;
				}
			}
		}
	}

	return;
}
