Pour tester:
gcc -Wall -Wextra -Werror -g \
    main.c \
    lexer/*.c \
    parser/*.c \
    utils/*.c \
    ../Libft/*.c \
    -Iincludes \
    -o minishell \
    -lreadline

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
Commande : cat < input.txt
Effet : Le texte "hello" est écrit dans file.txt, en écrasant le contenu précédent.

Dans ta structure : outfile = "file.txt";  append = 0;

- ">> : outfile (ajout)"
Commande : echo hello >> file.txt
Effet : "hello" est ajouté à la fin de file.txt, sans effacer son contenu.
Dans ta structure : outfile = "file.txt";  append = 1;

- << : heredoc (redirection d’entrée temporaire)
Commande : cat << END bonjour au revoir END
Effet : Tout ce que tu tapes entre cat << END et la ligne END est capturé.
Ce contenu est ensuite donné à la commande comme si c'était l'entrée standard (stdin).
Dans ta structure : heredoc = 1;  delimiter = "END";


CHECKLIST DÉTAILLÉE — Parsing & Signaux

🧱 PHASE 0 — Préparation du projet
Créer la structure du projet
* main.c
* minishell.h
* parser/ : lexer.c, parser.c, expander.c
* signals/ : signals.c
* utils/ : utils.c, errors.c
* Makefile
Configuration du Makefile

📟 PHASE 1 — Prompt et Signaux
Utiliser readline() pour afficher un prompt
* readline("minishell$ ")
* Ajouter chaque ligne non vide à l’historique avec add_history()
Gérer les signaux en mode interactif
* Ctrl-C (SIGINT) → affiche une nouvelle ligne propre avec prompt
* Ctrl-\ (SIGQUIT) → ignoré
* Ctrl-D → détecté si readline() retourne NULL (EOF), donc on sort du shell proprement
Créer un fichier signals.c
* Fonction setup_signals() avec sigaction() ou signal()
* Utiliser au plus une variable globale volatile sig_atomic_t g_signal

🧠 PHASE 2 — Lexer (tokenisation)
Analyser ligne en tokens (commande brute → tokens)
Créer lexer() :
* Respecter :
    * " → expansion activée
    * ' → pas d’expansion
* Gérer :
    * Pipes |
    * Redirections <, >, >>, <<
    * Mots simples (ls, cat, etc.)
* Pas besoin de gérer \ ou ;
Structure recommandée :
typedef enum e_token_type {
  WORD, PIPE, REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC
} t_token_type;

typedef struct s_token {
  char *value;
  t_token_type type;
  struct s_token *next;
} t_token;

🧱 PHASE 3 — Parser (construction des commandes)
Créer un parser() qui transforme la liste de t_token en t_cmd
c
CopierModifier
typedef struct s_cmd {
  char **args; // ex: {"ls", "-l", NULL}
  char *infile;     // pour <
  char *outfile;    // pour >
  int   append;     // 1 pour >>, 0 sinon
  int   heredoc;    // 1 pour <<
  char *delimiter;  // pour <<
  struct s_cmd *next; // pour le pipe
} t_cmd;
Gérer :
* redirections <, >, >>, <<
* séquence de pipes ls | wc | cat
* stockage des arguments (args) en respectant les quotes

💵 PHASE 4 — Expansion
Implémenter $ + expansion dans expander.c
* Gérer :
    * $VAR → getenv("VAR")
    * $? → status de la dernière commande
* Attention :
    * Entre '...' → rien n’est expansé
    * Entre "..." → oui, expansion
* Ajouter fonction expand_tokens(t_token *tokens, int last_status)

📜 PHASE 5 — Vérification syntaxique
 Avant de parser, valider les tokens :
* | en début ou fin
* <<, >>, <, > sans argument
* quotes non fermées
Créer : int check_syntax_errors(t_token *tokens);

💬 PHASE 6 — Heredoc (dans parsing)
 Gérer dans le parsing
* Lire les lignes jusqu’au délimiteur (sans expansion si entre quotes)
* Stocker dans pipe temporaire (utilisé par exec)

🧪 PHASE 7 — Tests & intégration avec exec
 Simuler les entrées pour ton binôme
* Tu peux afficher les t_cmd générées pour qu’il voie ce qu’il va exécuter
 Faire une fonction unique pour le parsing
t_cmd *parse_input(char *line, int last_exit_code);

🧽 PHASE 8 — Nettoyage & Valgrind
Libérer toute la mémoire
* Tokens
* Cmds
* Heredocs temporaires
* Expansion

🧠 Rappel des points clés du sujet :
✔️ 1 seule variable globale (pour signal)✔️ Respect strict des quotes✔️ readline + add_history()✔️ Ctrl-C, Ctrl-D, Ctrl-\✔️ Pas d’interprétation de ; ou \✔️ Expansion ($VAR, $?)✔️ Redirections + Pipes✔️ Syntaxe valide
