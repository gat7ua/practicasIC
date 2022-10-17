#include "header_files/global.hpp"
#include "header_files/HashMap.hpp"

using namespace std;

// Constructor de HPoint
HPoint::HPoint(int a, int b)
{
	x = a;
	y = b;
}

HPoint::HPoint()
{
	x = 0;
	y = 0;
}

HashMap::HashMap(void)
{
	this->table = new HPoint*[503];
}

int HashMap::hash(HPoint *data)
{
	return ((data->x + 79) * (data->y + 37) * 19) + 102793;
}

int HashMap::getSize(void)
{
	return this->size;
}

int HashMap::getCapacity(void)
{
	return this->capacity;
}

// Comprueba si el punto determinado por las coordenadas de entrada está en el mapa o no
int HashMap::contains(int a, int b)
{
	HPoint *temp = new HPoint(a, b);
	int hash = this->hash(temp);

	for (int i = 0; this->table[hash % this->capacity] != NULL; i++)
	{
		temp = this->table[hash % this->capacity];
		if (temp->x == a && temp->y == b)
			return 1;
		else
			hash += 2 * i + 1;
	}

	return 0;
}

// Inserta un punto en el mapa
void HashMap::insert(int a, int b)
{
	// Disallow duplicates
	if (this->contains(a, b))
		return;

	HPoint *newPoint = new HPoint(a, b);
	int hash = this->hash(newPoint);

	if (this->size > this->capacity / 2)
		this->expand();

	for (int i = 0; this->table[hash % this->capacity] != NULL; i++)
		hash += 2 * i + 1;

	this->table[hash % this->capacity] = newPoint;
	this->size++;

	return;
}

// Expande la tabla
void HashMap::expand(void)
{
	this->capacity = this->primes[++this->currentPrime];
	HPoint **newTable = new HPoint*[this->capacity];
	HPoint *temp;

	for (int i = 0; i < this->primes[this->currentPrime - 1]; i++)
	{
		temp = this->table[i];
		if (temp == NULL)
			continue; 

		int hash = this->hash(temp);

		newTable[hash % this->capacity] = temp;

	}

	this->table = newTable;
	return;
}

// Función auxiliar a la hora de debuggear
void HashMap::printTable(void)
{
	HPoint *temp;

	for (int i = 0; i < this->size; i++)
	{
		temp = this->table[i];
		if (temp == NULL)
			cout << "NULL, ";
		else
			cout << "(" << temp->x << ", " << temp->y << "), ";
	}

	cout << endl << "size is : " << this->size << " and capacity: " << this->capacity << endl;
	return;
}
