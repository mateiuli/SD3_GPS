#ifndef LIST_HPP
#define LIST_HPP

#include <stdio.h>

template <class T>
class List {
public:
	struct Node {
		/* Elementul retinut */
		T x;

		/* Elementul urmator */
		Node *next;
	};

private:
	Node *list;

public:
	/* Initializeaza lista */
	List();

	/* Goleste memoria */
	~List();

	/* Adauga element nou */
	void push(T x);

	/* Scoate un element din lista */
	T pop();

	/* Cauta in lista nodul 'nod' */
	T *find(int nod, bool (*cmp)(T x, int node)) const;

	/* Lista vida */
	bool isEmpty() const;

	/* Afiseaza elementele listei */
	void print() const;

	/* Adresa listei */
	Node *begin() const;
};

// template-urile trebuie implementate in header
#include "List.cpp"

#endif