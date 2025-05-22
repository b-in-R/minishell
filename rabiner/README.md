
** Minishell **

Parsing:	
	lexical analysis / tokenisation:

			taking input from user and processing char by char into tokens
			! what is important is the type of lexems/tokens, not the value !

			!! (generating an AST - abstract syntax tree)



Execution:
	taking the signals and executing


		!!Precedence climbing:
			voir l'ampleur des operations (chaque entree est un token)

			2 + 3 ^ 4 * 5 - 6
			|---------------| 	: amp 1 = 2 + ((3^4)*5) - 6
			    |-------|     	: amp 2 = (3^4) * 5
				|---|			: amp 3 =  3 ^ 4


			typedef struct s_ampl
			{
				int	n;
				int	pos;
			}		t_ampl;


			int	find__next_ampl_pos(char *token, char to_find_1, int *i)
			{
				int	pos;

				while (token[*i] != to_find_1)
				{
					*i++;
				}
				pos = *i;// a voir
				return (pos);
			}

			{
				int i = 0;
				t_ampl	ampl_1;
				t_ampl	ampl_2;
				t_ampl	ampl_3;
				int	nbr = 0;
				int	res = 0;

				// chercher le nombre d'"ampleur"
				while (token[i] != NULL)
				{
					if (token[i] == '+' || token[i] == '-')
					{
						ampl_1.n++;
					}
					else if (token[i] == '*' || token[i] == '/')
					{
						ampl_2.n++;
					}
					else if (token[i] == '^')
					{
						ampl_3.n++;
					}
					i++;
				}

				// calculs selon l'ampleur
				i = 0;
				while (ampl_1.n != 0)
				{
					while (ampl_2.n != 0)
					{
						while (ampl_3.n != 0)
						{
							ampl_3.pos = find_next_ampl_pos(token, '^', &i);
							res = (token[ampl_3.pos - 1] ^ token[ampl_3.pos + 1]);
							token[ampl_3.pos - 1] = res;
							token[ampl_3.pos + 1] = res;
							ampl_3.n--;
							if (ampl_3.n == 0)
								i = 0;
						}
						if (token[i] == '*')
						{
							ampl_2.pos = find_next_ampl_pos(token, '*', &i);
							res = (token[ampl_2.pos - 1] * token[ampl_2.pos + 1]);
							token[ampl_2.pos - 1] = res;
							token[ampl_2.pos + 1] = res;
							ampl_2.n--;
						}
						else if (token[i] == '/')
						{
							ampl_2.pos = find_next_ampl_pos(token, '/', &i);
							res = res + (token[ampl_2.pos - 1] / token[ampl_2.pos + 1]);
							token[ampl_2.pos - 1] = res;
							token[ampl_2.pos + 1] = res;
							ampl_2.n--;
						}
						if (ampl_2.n == 0)
							i = 0;
					}
					if (token[i] == '+')
					{
						ampl_1.pos = find_next_ampl_pos(token, '+', &i);
						res = (token[ampl_1.pos - 1] + token[ampl_1.pos + 1]);
						token[ampl_1.pos - 1] = res;
						token[ampl_1.pos + 1] = res;
						ampl_1.n--;
					}
					else if (token[i] == '-')
					{
						ampl_1.pos = find_next_ampl_pos(token, '-', &i);
						res = res + (token[ampl_1.pos - 1] - token[ampl_1.pos + 1]);
						token[ampl_1.pos - 1] = res;
						token[ampl_1.pos + 1] = res;
						ampl_1.n--;
					}
					if (ampl_1.n == 0)
						i = 0;
				}
			}



		exemples pseudo-code:

			int execute(t_node node)
			{
				if (node.type == PIPE)
					return (execute_pipe(node.left, node.right));
				else
					return (execute_simple_command(node.value))
			}


			int execute_pipe()
			{
				pipe(pipe_fds);
				left_pid = fork();
				if (pid == 0) // inside the left child
				{
   					// do stuff
   					close(pipe_fds[0]); //Accordingly
   					close(pipe_fds[1]); //Accordingly
				}
				else // back to the parent
				{
				    //do the same for the right child
    				// Make sure to close the pipe_fds in the parent too
   				}
			}