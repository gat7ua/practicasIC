#ifndef HASHMAP_HPP
#define HASHMAP_HPP

// Clase HPoint que se usará en HasMap
class HPoint
{
public:
	int x, y;
	HPoint(int, int);
	HPoint(void);
};

// Para esta implementación, no necesitaremos la función delete
class HashMap
{
	public:
		HashMap(void);
		void insert(int, int);
		int contains(int, int);
		int getSize(void);
		int getCapacity(void);
		void printTable(void);
	private:
		HPoint **table;
		void expand();
		int hash(HPoint*);
		int size = 0;
		int capacity = 503;
		int currentPrime = 0;
		int primes[10] = {503, 1009, 2027, 4637, 10061, 22391, 47161};
};

#endif
