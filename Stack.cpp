#include "Stack.hpp"
#include "Queue.hpp"

Stack *s_init()
{
	return NULL;
}

/* adauga la inceput o comanda noua */
void s_push(Stack* &S, Command *cmd)
{
	if(cmd == NULL)
		return;

	// creez un nod nou
	Stack *tmp = new Stack;
	tmp->prev = NULL;
	tmp->next = NULL;
	tmp->cmd = cmd;

	if(S == NULL) {
		S = tmp;
		return;
	}

	S->prev = tmp;
	tmp->next = S;
	S = tmp;
}

/* returneaza adresa catre comanda (nu o sterge din memorie) */
Command *s_pop(Stack* &S)
{
	if(S == NULL)
		return NULL;

	Command *cmd = S->cmd;
	Stack *tmp = S;
	S = S->next;
	if(S != NULL)
		S->prev = NULL;
	
	delete tmp;
	
	return cmd;
}

/* verifica daca stiva e goala */
bool s_isEmpty(Stack *S)
{
	return S == NULL;
}

/* debug: printeaza elementele din stiva de comenzi */
void s_print(Stack *S)
{
	char **c_args = NULL;

	while(S != NULL) {
		switch(S->cmd->type) {
			case GOTO_LINE:
				printf("[GL -> %d]\n", S->cmd->args[0]);
				break;

			case GOTO_CHAR:
				printf("[GC -> %d %d]\n", S->cmd->args[0], S->cmd->args[1]);
				break;

			case DELETE_LINE:
				printf("[DL -> %d]\n", S->cmd->args[0]);
				break;

			case DELETE:
				printf("[D -> %d]\n", S->cmd->args[0]);
				break;

			case BACKSPACE:
				printf("[backspace]\n");
				break;

			case TEXT_LINE:
				// afisez toata coada
				q_print((Queue*)S->cmd->info);
				break;

			case REPLACE:
				c_args = (char**)S->cmd->info;
				printf("[%s] -> [%s]\n", c_args[0], c_args[1]);
				break;

			default:
				printf("Other cmd\n");
		}
		
		S = S->next;
	}
}

/* goleste toata memoria ocupata de stiva */
void s_free(Stack *S)
{
	while(S != NULL) {
		Stack *next = S->next;

		// golesc zona in care e stocat stringul
		if(S->cmd->type == TEXT_LINE) {
			q_free((Queue*)S->cmd->info);
		}

		// golesc memoria ocupata de replace
		if(S->cmd->type == REPLACE) {
			char **c_args = NULL;
			c_args = (char**)S->cmd->info;

			for(int i = 0; i < MAX_ARGS; i++) {	
				// alocat cu strdup -> malloc
				free(c_args[i]);
			}

			delete[] (char**)S->cmd->info;
		}

		// sterg comanda
		delete S->cmd;

		// sterg nodul
		delete S;

		S = next;
	}
}

/* returneaza un pointer catre ultimul element */
Stack *s_end(Stack *S)
{
	while(S != NULL && S->next != NULL) {
		S = S->next;
	}

	return S;
}