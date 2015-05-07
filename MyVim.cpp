#include "MyVim.hpp"

int main(int argc, char **args)
{
	// tipul comenzii curente
	CMDType cmdType;

	// buffer pentru citire comanda
	char buff[BUFF_SIZE + 1];

	// argumentele comenzii
	int cmd_args[MAX_ARGS], cmds = 0;

	// lista cu comenzi de executat
	Stack *DoList = s_init();

	// lista cu comenzi anulate (UNDO)
	Stack *RedoList = s_init();

	// temporara, pentru pop
	Command *cmd = NULL;

	// stringurile pentru replace
	char **c_cmd_args = NULL;

	// salvez la 5 comenzi numai daca 
	// s-a salvat manual inainte o data
	bool saved = false;
	
	do {
		// ficare caracter e un element intr-o lista
		Queue *line = q_init();

		// citesc o comanda si o salvez in buff, iar linia
		// text (daca exista) se salveaza in coada line 
		cmdType = readCommand(buff, cmd_args, c_cmd_args, line);		
		
		// se pot primi doua comenzi in acelasi timp
		// una TEXT_LINE, iar cealalta o comanda ::<cmd>
		if(cmdType == TEXT_LINE || !q_isEmpty(line))
			s_push(DoList, newTextCmd(line));		

		switch(cmdType) {
			case UNDO:
				do {
					// preiau ultimele linii de text
					cmd = s_pop(DoList);
					
					// le pun in lista pentru redo
					s_push(RedoList, cmd);	

					if(s_isEmpty(DoList))
						break;

				} while(DoList->cmd->type == TEXT_LINE && cmd->type == TEXT_LINE);
				
				break;

			case REDO:
				do {
					// preiau ultimele linii de text scoase
					cmd = s_pop(RedoList);
					
					// le pun in lista pentru undo
					s_push(DoList, cmd);	

					if(s_isEmpty(RedoList))
						break;

				} while(RedoList->cmd->type == TEXT_LINE && cmd->type == TEXT_LINE);
				break;

			case REPLACE:
				s_push(DoList, newReplaceCmd(c_cmd_args));
				break;

			case SAVE:
				saved = true;
				save_to_file(DoList, args[1]);
				break;

			default:
				// do nothing
				break;
		}

		// adaug pe stiva si comanda ce trebuie executata
		if(isStackCommand(cmdType)) {
			s_push(DoList, newCommand(cmdType, NULL, cmd_args));
		}
		
		if(cmdType != TEXT_LINE && cmdType != INVALID)
			if(cmdType != SAVE && cmdType != QUIT)
				cmds++;

		// la 5 comenzi se salveaza in fisier
		if(cmds >= 5) {
			cmds = 0;
			if(saved)
				save_to_file(DoList, args[1]);
		}
	} while(cmdType != QUIT);

	// eliberez memoria
	s_free(DoList);
	s_free(RedoList);

	return 0;
}

/* construieste mesajul final pe baza comenzilor ramase */
Queue *buildOutput(Stack *DoList)
{
	// parcurg stiva de jos in sus
	Stack *p = s_end(DoList);

	// pointer catre caracterul curent
	Queue *c_pos = q_init();

	// santinela, caracter random MEMORY LEAK!
	q_push(c_pos, '\n');

	// parcurg stiva de jos in sus si execut comenzile ramase
	while(p != NULL) {
		switch(p->cmd->type) {
			case TEXT_LINE:
				// inserez textul in pozitia curenta, c_pos se modifica
				q_insert(c_pos, (Queue*)p->cmd->info);
				break;

			case BACKSPACE:
				q_delete_back(c_pos);
				break;

			case DELETE:
				q_delete_next(c_pos, p->cmd->args[0]);
				break;

			case DELETE_LINE:
				// mut cursorul pe linia ce trebuie stearsa
				if(p->cmd->args[0] > 0)  
					c_pos = goto_line(c_pos, p->cmd->args[0]);					

				// sterge linia
				q_delete_line(c_pos);
				break;

			case GOTO_LINE:
				c_pos = goto_line(c_pos, p->cmd->args[0]);
				break;

			case GOTO_CHAR:
				// fixez linia
				if(p->cmd->args[1] > 0) {
					c_pos = goto_line(c_pos, p->cmd->args[1]);
					if(c_pos != NULL)
						c_pos = c_pos->next;
				}

				// fixez caracterul in linia curenta
				c_pos = goto_char(c_pos, p->cmd->args[0]);
				break;

			case REPLACE:
				replace(c_pos, (char**)p->cmd->info);
				break;

			default:

				break;
		}
		
		p = p->prev;
	}

	return first(c_pos);
}

/* pune cursorul la inceputul liniei n */
Queue *goto_line(Queue *Q, int n)
{
	// ma duc la inceputul liste
	Q = first(Q);

	// caz particular linia 1
	if(n == 1)
		return Q;

	// sar peste santinela
	Q = Q->next;

	while(n - 1 > 0 && Q != NULL) {
		if(Q->c == '\n')
			n--;
		
		if(n - 1 <= 0)
			break;

		Q = Q->next;
	}

	return Q;
}

/* pune cursorul pe caracterul n  */
Queue *goto_char(Queue *Q, int n)
{
	// merg inapoi pana la primul caracter de pe linie
	while(Q != NULL && Q->prev != NULL) {
		Q = Q->prev;
		if(Q->c == '\n')
			break;
	}

	while(n - 1 > 0 && Q != NULL && Q->next != NULL) {
		n--;
		Q = Q->next;
	}

	return Q;
}

/* salveaza si genereaza output-ul */
void save_to_file(Stack *DoList, char *fileName)
{
	// generez output-ul (executa comenzile)
	Queue *out = buildOutput(DoList);

	// savleaza in fisier lista
	q_save(out->next, fileName);

	// elibereaza memoria 
	q_free(out);
}

/* inlocui prima aparitie a lui c_args[0] cu c_args[1] */
void replace(Queue *Q, char *c_args[])
{
	bool ok;
	int i;
	int length_old = strlen(c_args[0]);
	int length_new = strlen(c_args[1]);
	Queue *k, *p = Q;

	while(p != NULL) {
		// cautare noua
		ok = true;
		k = p;
		i = 0;

		while(k != NULL && i < length_old && ok) {
			if(k->c != c_args[0][i++])
				ok = false;
			k = k->next;
		}

		// primul gasit
		if(ok)
			break;

		p = p->next;
	}

	if(p != NULL)
		p = p->prev;

	// incepand cu p este textul cautat, il sterg
	q_delete_next(p, length_old);

	// creez o lista cu textul nou
	Queue *newText = q_init();
	for(i = 0; i < length_new; i++)
		q_push(newText, c_args[1][i]);

	// inserez noua lista
	q_insert_list(p, newText);
}