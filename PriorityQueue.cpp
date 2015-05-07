template <class T>
PriorityQueue<T>::PriorityQueue(int maxElem):
nrElem(0), V(new Item[maxElem+1])
{
}

template <class T>
PriorityQueue<T>::~PriorityQueue()
{
	delete[] V;
	nrElem = 0;
}

template <class T>
void PriorityQueue<T>::free()
{
	delete[] V;
	V = NULL;
	nrElem = 0;
}

template <class T>
void PriorityQueue<T>::push(T x, double priority)
{
	int k = ++nrElem;
	V[k].x = x;
	V[k].priority = priority;

	while(k > 1 && V[k/2].priority > V[k].priority) {
		swap(V[k/2], V[k]);
		k /= 2;
	}
}

template <class T>
T PriorityQueue<T>::pop()
{
	T item = V[1].x;
	V[1] = V[nrElem--];

	// ajusteaza arborele cu radacina in 1
	heapify(1);

	return item;
}

template <class T>
void PriorityQueue<T>::swap(Item &a, Item &b) const
{
	Item c = a;
	a = b;
	b = c;
}

template <class T>
int PriorityQueue<T>::min(int k) const
{
	int st = 2 * k;
	int dr = st + 1;
	int m = k;

	if(st <= nrElem && V[st].priority < V[m].priority)
		m = st;

	if(dr <= nrElem && V[dr].priority < V[m].priority)
		m = dr;

	return m;
}

template <class T>
void PriorityQueue<T>::heapify(int k)
{
	int m = min(k);

	if(V[m].priority != V[k].priority) {
		swap(V[k], V[m]);
		heapify(m);
	}
}

template <class T>
void PriorityQueue<T>::decreaseP(T x, double priority)
{
	bool found = false;
	int k = 0;

	// caut nodul x si ii micsorez prioritatea
	for(k = 1; k <= length(); k++)
		if(V[k].x == x) {
			V[k].priority = priority;
			found = true;
			break;
		}

	if(!found)
		return;
	
	// noua prioritate fiind mai mica trebuie
	// sa ajustez in sus coada cu prioritati
	while (k > 0) {
		int parent = k >> 1;

		if(V[parent].priority < V[k].priority)
			return;

		swap(V[k], V[parent]);
		k = parent;
	}
}

template <class T>
int PriorityQueue<T>::length() const
{
	return nrElem;
}

template <class T>
T PriorityQueue<T>::get(int k) const
{
	return V[k].x;
}

template <class T>
double PriorityQueue<T>::getP(int k) const
{
	return V[k].priority;
}

template <class T>
bool PriorityQueue<T>::isEmpty() const
{
	return nrElem == 0;
}