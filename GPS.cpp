#include "GPS.hpp"

int main(int argc, char **args)
{
	List<Graf> *G = NULL;

	// citesc reteaua de strazi din fisier
	int N = readGraph(args[2], G);

	if(N < 0 || argc < 3)
		return 0;

	// buffer citire: origine, destinate, distanta
	NodePair addr[2];

	// coordonatele punctul de inceput si final
	for(int i = 0; i < 2; i++)
		scanf("%d%d%lf", &addr[i].start, &addr[i].end, &addr[i].offset);

	// verific validitatea nodurilor
	for(int i = 0; i < 2; i++) {
		if(addr[i].start > N || addr[i].end > N) {
			printMsg(INV_ADDRESS);
			deleteGraph(G);
			return 0;
		}
	}
	
	// rezolv cerintele
	short task = atoi(args[1]);

	if(task <= 2) {
		// pentru task 2 afisez mesaje interactive
		doTask(G, N, addr, task == 2 ? true : false);
	} else {
		// interschimb nodurile pentru ca
		// la task-ul 3 sunt date invers
		NodePair tmp = addr[0];
		addr[0] = addr[1];
		addr[1] = tmp;

		doTask3(G, N, addr);
	}

	// eliberez memoria ocupata de graf
	deleteGraph(G);

	return 0;
}

/**
* Rezolva prima cerinta a problemei
* addr[0]: adresa de inceput
* addr[1]: adresa de final
*/
void doTask(List<Graf> *G, int N, NodePair addr[], bool printText)
{
	// distantele intre intersectiile dintre
	// care se pleaca si se ajunge
	double d_start = getDistance(G, addr[0].start, addr[0].end);
	double d_end = getDistance(G, addr[1].start, addr[1].end);
	
	// verific daca traseul este posibil
	if(addr[0].offset > d_start || addr[1].offset > d_end) {
		printMsg(INV_ADDRESS);
		return;
	}

	// stabilesc care sunt nodurile de inceput/final
	// in functie de distantele date
	NodePair pair;
	// nodul de unde porneste
	pair.start = addr[0].end;
	// nodul in care trebuie sa ajunga
	pair.end = addr[1].start;
	
	// daca se cere o distanta intre aceleasi
	// doua intersectii, traseul este totusi posibil
	if(addr[0].start == addr[1].start && addr[0].end == addr[1].end) {
		double d = addr[1].offset - addr[0].offset;
		if(d > 0) {
			printMsg(GO_STRAIGHT_DIST, d);
			printMsg(DESTINATION);
			return;
		}
	}

	// caut cel mai scurt drum
	MinPath *path = shortestPath(G, N, pair);

	// in caz ca nu exista drum
	if(path->dist[pair.end] >= INF) {
		printMsg(NO_ROUTE);
		freeMinPath(path);
		return;
	}

	// distanta minima finala
	double d = path->dist[pair.end];
	// includ offset-ul fata de intersectii
	d += d_start + addr[1].offset - addr[0].offset;

	if(!printText) {
		// taskul 1
		printf("%.3lf ", d);
		printPath(path, pair.end);
		printf("\n");
	} else {
		// generez vectorul de succesori
		getSuccessors(path, addr, N);
		printInd(G, path, addr);
	}

	freeMinPath(path);
}

/** 
* Calculeaza traseul cel mai scurt de la nodul 'start' la nodul 'end'
* Returneaza distantele de la nodul de inceput la toate celelalte noduri
* Intoarce un vector de predecesori pentru reconstituirea traseului
* pair: nodul start si nodul final din cautarea drumului minim
*/
MinPath *shortestPath(List<Graf> *G, int N, NodePair pair)
{
	MinPath *path = new MinPath;

	// distantele de la start
	path->dist = new double[N];

	// vectori de predecesori
	path->pred = new int[N];

	// vector de succesori
	path->succ = new int[N];

	// vector de vizitare
	bool *viz = new bool[N];

	// generez coada de prioritati
	PriorityQueue<int> Q(N);

	// distantele de la start la toate celelalte noduri
	path->dist[pair.start] = 0;

	for(int i = 0; i < N; i++) {
		if(i != pair.start) 
			path->dist[i] = INF;
		
		path->pred[i] = path->succ[i] = -1;
		viz[i] = false;

		// adaug nodul i cu prioritatea dist[i]
		Q.push(i, path->dist[i]);
	}

	while(!Q.isEmpty()) {
		// exterg nodul cu distanta minima
		int nod = Q.pop();

		if(viz[nod])
			continue;

		// vizitez nodul
		viz[nod] = true;
		
		// parcurg vecinii nodului extras
		List<Graf>::Node *it = G[nod].begin();

		while(it != NULL) {
			// calculez o posibila distanta mai scurta
			double alt = path->dist[nod] + it->x.dist;

			// daca am gasit-o, o retin si ajustez coada
			if(alt < path->dist[it->x.nod]) {
				path->dist[it->x.nod] = alt;
				path->pred[it->x.nod] = nod;
				Q.decreaseP(it->x.nod, alt);
			}
		
			it = it->next;
		}
	}
	
	delete[] viz;
	return path;
}

/**
* Rezolva task-ul 3 al problemei: mesaje interactive
* addr[1] = final, addr[0] = start
*/
void doTask3(List<Graf> *G, int N, NodePair addr[])
{
	// pereche inceput - destinatie
	NodePair pair;
	pair.start = addr[0].end;
	pair.end = addr[1].start;
	
	MsgType response = UNDEFINED;

	// caut un drum minim
   MinPath *path = shortestPath(G, N, pair);
   getSuccessors(path, addr, N, true);
	
	do {	
		// reconfigurare traseu daca e necesar
		if(!path->inPath[pair.start]) {
			freeMinPath(path);

			// caut un drum minim nou
			path = shortestPath(G, N, pair);
			getSuccessors(path, addr, N, true);
			printMsg(REROUTE);
      }

		// urmatoarea indicatie
		response = nextInd(G, path, addr);
	
		if(response == DESTINATION)
			break;

		// citesc pozitia curenta
		if(scanf("%d%d%lf", &addr[0].start, &addr[0].end, &addr[0].offset) != 3)
			break;

		pair.start = addr[0].end;

	} while(response != DESTINATION);

	freeMinPath(path);
}

/**
* Citeste reteaua de intersectii din fisierul dat ca parametru
*/
int readGraph(char *fileName, List<Graf> *&G)
{
	FILE *f = fopen(fileName, "r");

	if(!f)
		return -1;

	int N, nr;
	Graf tmp;

	// aloc cata memorie este necesara
	fscanf(f, "%d", &N);
	G = new List<Graf>[N];

	for(int i = 0; i < N; i++) {
		// numarul de vecini
		fscanf(f, "%d", &nr);

		for(int j = 0; j < nr; j++) {
			fscanf(f, "%lf%d", &tmp.dist, &tmp.nod);
			G[i].push(tmp);
		}
	}

	fclose(f);
	return N;
}

/**
* Returneaza distanta dintre doua noduri
*/
double getDistance(List<Graf> *G, int a, int b)
{
	Graf *nod = G[a].find(b, cmpGraph);
	return nod == NULL ? INF : nod->dist;
}

/**
* Goleste intreaga memorie ocupata de graf
*/
void deleteGraph(List<Graf>* &G)
{
	delete[] G;
	G = NULL;
}

/**
* Afiseaza mesajele interactive
*/
void printMsg(MsgType msgType, double dist, int node)
{
	char msg[80] = {'\0'};

	switch(msgType) {
		case INV_ADDRESS:
			strcpy(msg, "adresa invalida");
			break;

		case NO_ROUTE:
			strcpy(msg, "traseu imposibil");
			break;

		case GO_STRAIGHT:
			strcpy(msg, "mergeti inainte");
			break;

		case GO_STRAIGHT_DIST:
			sprintf(msg, "mergeti inainte %.3lf km", dist);
			break;

		case TURN_ON_STR:
			sprintf(msg, "virati catre %d", node);
			break;

		case TURN_ON_STR_DIST:
			sprintf(msg, "in %.0lf metri virati catre %d", dist * 1000, node);
			break;

		case DIST_DESTINATION:
			sprintf(msg, "in %.0lf metri ati ajuns la destinatie", dist * 1000);
			break;

		case DESTINATION:
			strcpy(msg, "ati ajuns la destinatie");
			break;

		case REROUTE:
			strcpy(msg, "reconfigurare traseu");
			break;

		default:
			break;
	}

	if(strlen(msg) == 0)
		return;

	printf("%s\n", msg);
}

/**
* Afiseaza drumul in ordine inversa
*/
void printPath(MinPath *path, int k)
{		
	if(k < 0) 
		return;

	printPath(path, path->pred[k]);
	printf("%d ", k);
}

/**
* Afiseaza indicatii numai pentru urmatoarea actiune (pt bonus)
*/
MsgType nextInd(List<Graf> *G, MinPath *&path, NodePair addr[])
{
	int nod = addr[0].start;
	double d = 0;

	if(path->succ[nod] == addr[1].end)
		d = addr[1].offset;
	else
		d = getDistance(G, nod, path->succ[nod]);

	d -= addr[0].offset;

	if(d >= 0.500) {
		printMsg(GO_STRAIGHT);
		return GO_STRAIGHT;
	}

	if(path->succ[addr[0].end] == -1) {
		if(d <= 0.025) {
			printMsg(DESTINATION);
			return DESTINATION;
		}

		printMsg(DIST_DESTINATION, d);
		return DIST_DESTINATION;	
	}

	printMsg(TURN_ON_STR_DIST, d, path->succ[addr[0].end]);
	return UNDEFINED;
}

/**
* Afiseaza indicatii pentru a ajunge la destinatie
*/
void printInd(List<Graf> *G, MinPath *path, NodePair addr[])
{		
	// distanta dintre intersectiile de unde se pleaca
	double d = getDistance(G, addr[0].start, addr[0].end);

	// mergi inainte
	printMsg(GO_STRAIGHT_DIST, d - addr[0].offset);

	// virati catre
	printMsg(TURN_ON_STR, -1, path->succ[addr[0].end]);

	int nod = addr[0].end;

	// traseu intermediar
	while(path->succ[nod] != -1 && path->succ[nod] != addr[1].end) {
		// distanta dintre 2 intersectii succesive
		d = path->dist[path->succ[nod]] - path->dist[nod];

		// mergi inainte d km
		printMsg(GO_STRAIGHT_DIST, d);

		// in caz ca nu e nodul destinatie
		if(path->succ[path->succ[nod]] != -1)
			printMsg(TURN_ON_STR, -1, path->succ[path->succ[nod]]); 

		nod = path->succ[nod];
	}

	// destinatia
	printMsg(GO_STRAIGHT_DIST, addr[1].offset);
	printMsg(DESTINATION);
}

/**
* Genereaza vectorul complet de succesori
*/
void getSuccessors(MinPath *&path, NodePair addr[], int N, bool inPath)
{
	// salvare noduri ce face parte din drumul minim
	if(inPath) {
		path->inPath = new bool[N + 1];
		// initializare
		for(int i = 0; i < N; i++)
			path->inPath[i] = false;

		// nodul final face parte din traseu
		path->inPath[addr[1].end] = true;
	}
	
	pred2succ(path, addr[1].start, inPath);
	path->succ[addr[0].start] = addr[0].end;
	path->succ[addr[1].start] = addr[1].end;
}

/**
* Vector predecesori --> vector succesori
*/
void pred2succ(MinPath *&path, int k, bool inPath)
{
	if(k < 0)
		return;

	if(inPath)
		path->inPath[k] = true;
	
	pred2succ(path, path->pred[k], inPath);

	if(path->pred[k] >= 0)
		path->succ[path->pred[k]] = k;
}

/**
* Compara un nod din graf cu un indice
*/
bool cmpGraph(Graf a, int nod)
{
	return a.nod == nod;
}

/**
* Goleste memoria ocupata de un drum minim
*/
void freeMinPath(MinPath *&path)
{
	delete[] path->dist;
	delete[] path->pred;
	delete[] path->succ;
	delete[] path->inPath;
	delete path;
	path = NULL;
}
