#include "Command.hpp"
#include "Queue.hpp"

/* Creeaza o comanda noua cu tipul si informatia specifica*/
Command *newCommand(CMDType type, void *info, int args[])
{
	Command *cmd = new Command;
	cmd->type = type;
	cmd->info = info;

	for(int i = 0; i < MAX_ARGS; i++)
		cmd->args[i] = args[i];
	
	return cmd;
}

/* Alias pentru comanda tip text */
Command *newTextCmd(Queue *info)
{
	int args[] = {-1, -1};
	return newCommand(TEXT_LINE, (Queue*)info, args);
}

/* Alias pentru comanda replace */
Command *newReplaceCmd(char *c_args[])
{
	int args[] = {-1, -1};
	return newCommand(REPLACE, (char**)c_args, args);
}

/* Citeste inputul, separa textul de comenzi */
CMDType readCommand(char *input, int args[], char **&c_args, Queue *&line)
{
	// ficare caracter e un element intr-o lista
	line = q_init();

	char c;
	int i = 0, is_cmd = 0;
	
	do {
		// citire caracter cu caracter
		if(scanf("%c", &c) == EOF)
			break;			
		
		// comanda se salveaza in buffer, linia text in coada
		if(is_cmd >= 2) 
			input[i++] = c;				
		else 
			if(c != ':')
				q_push(line, c);
		
		if(c == ':')
			is_cmd++;

	} while(c != '\n');

	// in input va fi stocata comanda (::<cmd> [arg1] [arg2])
	input[i] = '\0';

	// in caz ca nu s-a citit o comanda
	if(strlen(input) == 0) 
		return TEXT_LINE;

	char buff[5], rargs[MAX_ARGS][MAX_REPLACE] = {{0}};

	// preiau numele comenzii si parametrii (daca exista)
	sscanf(input, "%s%s%s", buff, rargs[0], rargs[1]);
	
	if(strcmp(buff, "re") != 0) {
		for(i = 0; i < MAX_ARGS; i++) {
			// argumentele sunt numere
			args[i] = atoi(rargs[i]);
	
			// valorile default
			args[i] = args[i] <= 0 ? -1 : args[i];
		}		
	} 
	else {
		c_args = new char*[MAX_ARGS];
		c_args[0] = strdup(rargs[0]);
		c_args[1] = strdup(rargs[1]);
	}

	if(strcmp(buff, "q") == 0)
		return QUIT;

	if(strcmp(buff, "u") == 0)
		return UNDO;

	if(strcmp(buff, "r") == 0)
		return REDO;

	if(strcmp(buff, "s") == 0)
		return SAVE;
	
	if(strcmp(buff, "b") == 0)
		return BACKSPACE;

	if(strcmp(buff, "dl") == 0)
		return DELETE_LINE;

	if(strcmp(buff, "gl") == 0)
		return GOTO_LINE;

	if(strcmp(buff, "gc") == 0)
		return GOTO_CHAR;

	if(strcmp(buff, "d") == 0) {
		args[0] = args[0] < 0 ? 1 : args[0];
		return DELETE;
	}

	if(strcmp(buff, "re") == 0)
		return REPLACE;

	return INVALID;
}

/* 
	Verifica daca comanda data trebuie salvata pe 
	stiva de comenzi ce trebuie executate
*/
bool isStackCommand(CMDType cmd)
{
	if(cmd == SAVE)
		return false;

	if(cmd == QUIT)
		return false;

	if(cmd == UNDO)
		return false;

	if(cmd == REDO)
		return false;

	if(cmd == INVALID)
		return false;

	if(cmd == REPLACE)
		return false;

	// linia text se pune pe stiva, dar nu se 
	// pune aici, se pune separat, la alta comanda
	if(cmd == TEXT_LINE)
		return false;

	return true;
}
