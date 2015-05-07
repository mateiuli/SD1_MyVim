#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdlib.h>
#include <stdio.h>
#include "Command.hpp"

struct Queue {
	char 	c;
	Queue *next;
	Queue *prev;
};

/* Initializeaza coada */
Queue *q_init();

/* Adauga un caracter in coada */
void q_push(Queue *&Q, char c);

/* Scoate un element din coada */
char q_pop(Queue *&Q);

/* Verifica daca coada e goala */
bool q_isEmpty(Queue *Q);

/* Afiseaza continutul cozii (util pentru debug) */
void q_print(Queue *Q);

/* Goleste memoria ocupata de coada*/
void q_free(Queue *Q);

/* Insereaza in coada la dreapta lui dest */
int q_insert(Queue *&dest, Queue *src);

/* Insereaza o coada la dreapta (shallow copy) */
void q_insert_list(Queue *&dest, Queue *src);

/* Salveaza in fisier continutul cozii */
void q_save(Queue *Q, char *fileName);

/* Returneaza pointer catre primul element din coada */
Queue *first(Queue *Q);

/* Sterge elementul Q */
void q_delete_back(Queue *&Q);

/* Sterge elementele de dupa Q pana la new-line */
void q_delete_line(Queue *&Q);

/* Sterge n elemente de dupa Q */
void q_delete_next(Queue *&Q, int n);

#endif
