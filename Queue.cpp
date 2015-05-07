#include "Queue.hpp"

/* initializeaza coada */
Queue *q_init()
{
	return NULL;
}

/* adauga un nou caracter in coada -> la final */
void q_push(Queue * &Q, char c)
{
	Queue *tmp = new Queue;
	tmp->c = c;
	tmp->next = NULL;
	tmp->prev = NULL;

	if(Q == NULL) {
		Q = tmp;
		return;
	}

	Queue *p = Q;
	while(p != NULL && p->next != NULL) {
		p = p->next;
	}

	// p e ultimul element
	p->next = tmp;
	tmp->prev = p;
}

/* scoate primul element din coada */
char q_pop(Queue * &Q)
{
	if(Q == NULL)
		return 0;
	
	int v = Q->c;
	Queue *next = Q->next;
	delete Q;
	Q = next;
	return v;
}

/* printeaza lista cu elemente */
void q_print(Queue *Q)
{
	while(Q != NULL) {
		printf("%c", Q->c);
		Q = Q->next;
	}
}

/* verifica daca lista e goala */
bool q_isEmpty(Queue *Q)
{
	return Q == NULL;
}

/* elibereaza memoria ocupata de lista */
void q_free(Queue *Q)
{
	while(Q != NULL) {
		Queue *next = Q->next;
		delete Q;
		Q = next;
	}
}

/* 
	copiaza fiecare element din src in dest (deep copy) 
	returneaza numarul de caractere copiate
*/
int q_insert(Queue *&dest, Queue *src)
{
	int len = 0;

	// creez o lista noua cu elementele ce trebuie copiate
	Queue *tmp = q_init();
	while(src != NULL) {
		q_push(tmp, src->c);
		src = src->next;
		len++;
	}

	// leg noua lista de lista veche
	Queue *next = NULL;
	if(dest != NULL) {
		next = dest->next;
		dest->next = tmp;	
	}

	tmp->prev = dest;
	
	// parcurg lista pana la final
	while(tmp != NULL && tmp->next != NULL) 
		tmp = tmp->next;	

	tmp->next = next;
	dest = tmp;
	if(next != NULL)
		next->prev = tmp;

	return len;
}

/* insereaza lista direct - shallow copy */
void q_insert_list(Queue *&dest, Queue *tmp)
{
	// leg noua lista de lista veche
	Queue *next = NULL;
	if(dest != NULL) {
		next = dest->next;
		dest->next = tmp;	
	}

	tmp->prev = dest;
	
	// parcurg lista pana la final
	while(tmp != NULL && tmp->next != NULL) 
		tmp = tmp->next;	

	tmp->next = next;
	dest = tmp;
	if(next != NULL)
		next->prev = tmp;
}

/* salveaza continutul listei in fisier */
void q_save(Queue *Q, char *fileName)
{
	FILE *f = fopen(fileName, "w");
	
	if(!f)
		return;

	while(Q != NULL) {
		fprintf(f, "%c", Q->c);
		Q = Q->next;
	}

	fclose(f);
}

/* returneaza pointer catre primul element */
Queue *first(Queue *Q) 
{
	while(Q != NULL && Q->prev != NULL) {
		Q = Q->prev;
	}
	return Q;
}

/* sterge elementul indicat de pointer si reface legaturile */
void q_delete_back(Queue *&Q)
{
	if(Q == NULL)
		return;

	Queue *tmp = Q;
	if(Q->prev != NULL)
		Q->prev->next = Q->next;
	
	if(Q->next != NULL)
		Q->next->prev = Q->prev;

	// backspace -> merge inapoi
	Q = Q->prev;

	delete tmp;
}

/* sterge urmatoarele n elemente */
void q_delete_next(Queue *&Q, int n)
{
	if(Q == NULL)
		return;

	while(n-- > 0 && Q->next != NULL) {
		Q = Q->next;
		q_delete_back(Q);
	}
}

/* sterge linia curenta */
void q_delete_line(Queue *&Q)
{
	if(Q == NULL)
		return;
	
	char c;
	do {
		c = Q->next->c;
		q_delete_next(Q, 1);
		if(c == '\n')
			break;
	} while(Q != NULL && Q->next != NULL && Q->next->c != '\n');

	if(c != '\n')
		q_delete_next(Q, 1);

	return;
}