#ifndef GPS_HPP
#define GPS_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PriorityQueue.hpp"
#include "List.hpp"

#define INF (1<<30)
#define MAX_ELEM 10000

enum MsgType {
	INV_ADDRESS,
	NO_ROUTE,
	GO_STRAIGHT,
	GO_STRAIGHT_DIST,
	TURN_ON_STR,
	TURN_ON_STR_DIST,
	DIST_DESTINATION,
	DESTINATION,
	REROUTE,
	UNDEFINED,
};

struct Graf {
	int nod;
	double dist;
};

struct MinPath {
	// vector predecesori
	int *pred;
	// vector succesori
	int *succ;
	// distantele minime 
	double *dist;
	// noduri din drumul minim
	bool *inPath;

	MinPath(): pred(NULL), succ(NULL), dist(NULL), inPath(NULL) {}
};

struct NodePair {
	int start;
	int end;
	double offset;
};

/* Citeste graful din fisier */
int readGraph(char *fileName, List<Graf> *&G);

/* Elibereaza toata memoria ocupata de graf */
void deleteGraph(List<Graf> *&G);

/* Afiseaza mesaje interactive */
void printMsg(MsgType msg, double dist = -1.f, int node = -1);

/* Dijkstra */
MinPath *shortestPath(List<Graf> *G, int N, NodePair);

/* Returneaza distanta de la a la b sau infinit daca un exista */
double getDistance(List<Graf> *G, int a, int b);

/* Compara un nod cu un identificator al unui nod */
bool cmpGraph(Graf a, int nod);

/* Rezolva cerintele 1, 2 */
void doTask(List<Graf> *G, int N, NodePair addr[], bool printText = false);

/* Rezolva cerinta 3 (bonusul) */
void doTask3(List<Graf> *G, int N, NodePair addr[]);

/* Afiseaza drumul care se termina cu nodul k */
void printPath(MinPath *path, int k);

/* Afiseaza mesajele de ghidare pentru task-ul 2 */
void printInd(List<Graf> *G, MinPath *path, NodePair addr[]);

/* Urmatorul pas pentru task-ul 3 */
MsgType nextInd(List<Graf> *G, MinPath *&path, NodePair addr[]);

/* Vectorul de succesori */
void getSuccessors(MinPath *&path, NodePair add[], int N, bool inPath = false);

/* Genereaza recursiv vectorul de succesori */
void pred2succ(MinPath *&path, int k, bool inPath);

/* Goleste memoria unui drum minim */
void freeMinPath(MinPath *&path);

#endif