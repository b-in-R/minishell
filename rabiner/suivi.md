
===============================================================================

EN COURS:

	- codes d'erreur:	renvoi le code int last_status des commandes 
		( echo $? )


	- valgrind

	- echo "'salut'"	bash: 'salut'		minishell:	salut



	- modif:
				var.c:	str_append_free


===============================================================================

Malloc / free


CTRL+D:

	t_cmd *current->args;

	==45906==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
	==45906==    by 0x10CA01: add_arg (create_cmd.c:101)	// malloc + strdup ici
	==45906==    by 0x10CB04: process_token (parser.c:45)
	==45906==    by 0x10CC7C: parser (parser.c:81)	// initialisation t_cmd *current
	==45906==    by 0x1094D7: process_input (main.c:39)
	==45906==    by 0x1095F9: main (main.c:75)




===============================================================================

open / close





===============================================================================


BUGS:

	- valgrind


	- echo bonjour |;		bash:	bash: syntax error near unexpected token `;'
									code: [2]
							mini:	minishell: command not found: ;
									code: [2] + last_status[127] ??


===============================================================================

ERREURS SORTIE DE COMMANDES:

Commande:			Result:

							
echo bonjour |;		bash:	bash: syntax error near unexpected token `;'
							code: [2]
					mini:	minishell: command not found: ;
							code: [2] + last_status[127] ??


===============================================================================

GENERAL:

	-	gestion signaux dans child et parent, ne doit pas reagir comme un shell
		en plein readline pendant waitpid
		--> bloquer/ignorer certains signaux temporairement dans les child
	
	-	textes d'erreur a preciser

	-	free - gestion memoire (voir process avec top?, valgrind?)


===============================================================================

INFOS A TROUVER:

	-	heredoc

	- 	num de code de sortie des commandes (int last_satuts) echo $?	:
			1	Erreur générale    Une commande a échoué sans précision
			2	Mauvais usage de la commande (invalid option)    grep --badoption
			126 Commande trouvée mais non exécutable    chmod -x script.sh && ./script.sh
			127 Commande introuvable    toto (commande inconnue)
			128 Erreur de signal    Utilisé comme base pour les erreurs liées à des signaux
			130 Interruption par Ctrl+C (SIGINT)    Tu fais sleep 10, tu tapes Ctrl+C
			139	Erreur de segmentation (Segmentation fault) Un programme
				plante à cause de la mémoire



	VAR=val seul ⇒		crée/maj une variable de shell (locale au shell). Elle n’apparaît
						pas dans env tant que tu ne l’as pas exportée.

	export VAR (ou export VAR=val) ⇒	marque la variable pour qu’elle fasse partie de 
										l’environnement transmis aux processus enfants. 
										Elle apparaît dans env.

	unset VAR ⇒			supprime la variable (et son export éventuel).

	VAR=val cmd ⇒		n’exporte rien “de façon globale” : ça injecte VAR=val uniquement
						dans l’environnement de cmd (et de ses enfants), sans modifier
						l’état du shell après l’exécution.

	env ⇒ 				affiche l’environnement courant (les variables exportées + les
						assignments temporaires passés à env lui‑même).


===============================================================================
