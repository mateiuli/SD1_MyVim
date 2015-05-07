#ifndef STACK_HPP
#define STACK_HPP

#include <stdio.h>
#include <stdlib.h>
#include "Command.hpp"

struct Stack {
	Command	*cmd;
	Stack *next;
	Stack *prev;
};

/* Initializeaza stiva */
Stack *s_init();

/* Scoate ultima comanda din stiva*/
Command *s_pop(Stack *&S);

/* Adauga in stiva o comanda*/
void s_push(Stack *&S, Command *);

/* Returneaza pointer catre ultimul element */
Stack *s_end(Stack *S);

/* Verifica daca stiva e goala*/
bool s_isEmpty(Stack *S);

/* Returneaza numarul de elemente */
int s_length(Stack *S);

/* Afiseaza continutul stivei (util pentru debug)*/
void s_print(Stack *S);

/* Goleste memoria ocupata de stiva */
void s_free(Stack *S);

#endif 