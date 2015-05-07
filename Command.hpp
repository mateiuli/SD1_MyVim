#ifndef COMMMAND_HPP
#define COMMMAND_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Numarul maxim de argumente al unei comenzi */
#define MAX_ARGS 2

/* Lungimea maxima a textelor de la replace */
#define MAX_REPLACE 500

/* Tipuri de comenzi */
enum CMDType {
	SAVE,				
	QUIT,				
	UNDO,				
	REDO,				
	BACKSPACE,	
	GOTO_LINE,	
	GOTO_CHAR,	
	DELETE_LINE,
	DELETE,			
	REPLACE,
	TEXT_LINE,	
	INVALID,
};

struct Queue;

struct Command {
	/* tipul comenzii*/
	CMDType type;
	
	/* informatia cu care vine comanda */
	void *info;
	
	/* parametrii numerici ai comenzii (daca exista) */
	int args[MAX_ARGS];
};

/* Creeaza o comanda noua cu tipul dat */
Command *newCommand(CMDType type, void *info, int args[]);

/* Creeaza o comanda pentru o linie text */
Command *newTextCmd(Queue *info);

/* Creeaza o comanda pentru replace */
Command *newReplaceCmd(char *c_args[]);

/* citeste de la tastatura  */
CMDType readCommand(char *input, int args[], char **&c_args, Queue *&line);

/* comanda ce trebuie salvata sau executata */
bool isStackCommand(CMDType cmd);

#endif