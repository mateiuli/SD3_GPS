template <class T>
List<T>::List():
list(NULL)
{
}

template <class T>
List<T>::~List()
{
	while(!isEmpty())
		pop();
}

template <class T>
bool List<T>::isEmpty() const
{
	return list == NULL;
}

template <class T>
void List<T>::push(T x)
{
	Node *tmp = new Node;
	tmp->x = x;
	tmp->next = NULL;

	if(list == NULL) {
		list = tmp;
		return;
	}

	tmp->next = list;
	list = tmp;
}

template <class T>
T List<T>::pop()
{	
	T e;
	if(list == NULL)
		return e;

	e = list->x;
	Node *l = list;
	list = list->next;
	delete l;

	return e;
}

template <class T>
T* List<T>::find(int nod, bool (*cmp)(T x, int node)) const
{
	Node *l = list;
	while(l != NULL) {
		if(cmp(l->x, nod))
			return &l->x;
		l = l->next;
	}

	return NULL;
}

template <class T>
void List<T>::print() const
{
	Node *l = list;
	while(l != NULL) {
		printf("%d(%f) ", l->x.nod, l->x.dist);
		l = l->next;
	}

	printf("\n");
}

template <class T>
typename List<T>::Node* List<T>::begin() const
{
	return list;
}