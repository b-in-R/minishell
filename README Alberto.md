
Token usage in Minishell parsing process:

1. Lexer:
   Converts the raw input line (char *line) into a linked list of t_token elements.

2. Parser:
   Reads the list of t_token to build structured t_cmd elements
   (complete commands with arguments, redirections, pipes, etc.).

3. Expander:
   Replaces environment variables like $VAR and $? inside tokens of type WORD.

4. Validation:
   Checks for syntax errors (e.g., pipe at the beginning or end, missing redirection targets, unclosed quotes).

-> Tokens are an essential intermediate step between the raw user input and the structured commands to be executed.

REDIRECTIONS:

- < : infile (redirection d’entrée)
Commande : cat < input.txt
Effet : Le contenu du fichier input.txt remplace l’entrée standard (stdin).
Dans la structure t_cmd :  infile = "input.txt"

- "> : outfile (écrasement)"
Commande : echo hello > file.txt
Effet : Le texte "hello" est écrit dans file.txt, en écrasant le contenu précédent.

Dans ta structure : outfile = "file.txt";  append = 0;

- ">> : outfile (append)"
Commande : echo hello >> file.txt
Effet : "hello" est ajouté à la fin de file.txt, sans effacer son contenu.
Dans ta structure : outfile = "file.txt";  append = 1;

- << : heredoc (redirection d’entrée temporaire)
Commande : cat << END bonjour au revoir END
Effet : Tout ce que tu tapes entre cat << END et la ligne END est capturé.
Ce contenu est ensuite donné à la commande comme si c'était l'entrée standard (stdin).
Dans ta structure : heredoc = 1;  delimiter = "END";


/* EXAMPLE:
*t_token
* Used to parse the user's command line word by word.
* Each word (command, argument, symbol like |, <, etc.) is a token.
* This helps us understand the user's intent.
* → This is called grammatical analysis (like breaking a sentence into words and types).
*
*t_cmd
* Used to organize all this information for actual execution.
* Once the tokens are identified, we group them together:
* Arguments go together (ls, -l)
* We store redirections (e.g., > or >>)
* We link commands together if | is used
* → This is the complete meaning construct, ready to be executed
*
* Transition: from t_token to t_cmd in the parser
* E.g., line entered by the user:
* "ls -l | grep hello > out.txt"
*
* Step 1: The lexer produces this list of tokens:
* ["ls", WORD] → ["-l", WORD] → ["|", PIPE] → ["grep", WORD] → ["hello", WORD] → [">", REDIR_OUT] → ["out.txt", WORD]
*
* Step 2: The parser reads this list and constructs two t_cmds
* First command: ls -l
* t_cmd *cmd1;
* cmd1->args = {"ls", "-l", NULL};
* cmd1->outfile = NULL;
* cmd1->next = cmd2;
*
* Second command: grep hello > out.txt
* t_cmd *cmd2;
* cmd2->args = {"grep", "hello", NULL};
* cmd2->append = 0;
* cmd2->next = NULL;
*
* Conclusion:
* We parse (with t_token) to understand the user's input.
* We organize (with t_cmd) to prepare for execution.
* With this, we can call a function execute(t_cmd *cmds) and run the entire shell.
*/
