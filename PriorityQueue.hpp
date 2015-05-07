#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

#include <stdlib.h>

/* MinHeap */
template <class T>
class PriorityQueue {
private:
	/* Numarul curent de elemente */
	int nrElem;

	/* Elementul si prioritatea */
	struct Item {
		T x;
		double priority;

		Item(): priority(0) {}
	};
	
	/* Vectorul cu elemente */
	Item *V;

public:
	PriorityQueue(int maxElem);
	~PriorityQueue();

	/* Adauga un element nou in coada */
	void push(T x, double priority);

	/* Scoate cel mai mic element*/
	T pop();

	/* Numarul de elemente */
	int length() const;

	/* Valoarea elementului k */
	T get(int k) const;

	/* Prioritatea elementului k */
	double getP(int k) const;

	/* Verifica daca coada e vida */
	bool isEmpty() const;

	/* Micsoreaza prioritatea */
	void decreaseP(T k, double priority);

	/* Elibereaza memoria */
	void free();

private:
	/* Interschimba doua elemente */
	void swap(Item &a, Item &b) const;

	/* Ajusteaza arborele cu radacina in k */
	void heapify(int k);

	/* Valoarea minima a arborelui cu radacina in k */
	int min(int k) const;
};

#include "PriorityQueue.cpp"

#endif