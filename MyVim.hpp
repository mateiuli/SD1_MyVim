#ifndef MYVIM_HPP
#define MYVIM_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Command.hpp"
#include "Stack.hpp"
#include "Queue.hpp"

#define BUFF_SIZE 50

/* Executa comenzile din stiva */
Queue *buildOutput(Stack *DoList);

/* Pozitioneaza cursorul pe linia */
Queue *goto_line(Queue *Q, int n);

/* Pozitioneaza curosorul pe caracterul n din linia curenta*/
Queue *goto_char(Queue *Q, int n);

/* Executa comenzile si salveaza in fisier rezultatul */
void save_to_file(Stack *DoList, char *fileName);

/* Inlocuie in Q prima aparitie a lui c_args[0] cu c_args[1] */
void replace(Queue *Q, char *c_args[]);

#endif