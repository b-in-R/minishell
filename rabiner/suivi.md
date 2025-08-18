
===============================================================================

EN COURS:

	codes d'erreur:	renvoi le code int last_status des commandes ( echo $? )
						0  1  126  127  ..

	-  ' ou "	execve failed -> voir qui doit s'en occuper
				ne font pas le retour a la ligne avec > 
				voir handle_tokens.c -> handle quotes

	- valgrind
	
	- norminette

===============================================================================

BUGS:

	- valgrind


	-	minishell> grep "2" test.txt | awk "{print $2}'
		awk: 1: unexpected character '''

		(➜ minishell git:(rabiner) ✗ grep "2" test.txt | awk "{print $2}'
		pipe dquote> 
		(ctrl+c))

		' " ne font pas le retour a la ligne avec > 



===============================================================================

ERREURS SORTIE DE COMMANDES:

Commande:			Result:

| echo				bash:	bash: syntax error near unexpected token `|'
							code: [2]
					mini:	Syntax error: unexpected pipe
							code: [2]
							
echo bonjour |;		bash:	bash: syntax error near unexpected token `;'
							code: [2]
					mini:	minishell: command not found: ;
							code: [2] + last_status[127] ??

sleep 10 (+ctrl-c)	bash:	code: [130]
					mini:	code: [0] + last_status[130] ??
								--> voir a quel code est renvoye, si une modif
									de status est faite, recup cette valeur


===============================================================================

GENERAL:

	-	toutes les commandes builtin sont executees dans un fork
		--> quand il n'y a pas de pipe (1 seule cmd builtin), executer direct
			dans le parent (actuellement child)

	-	gestion signaux dans child et parent, ne doit pas reagir comme un shell
		en plein readline pendant waitpid
		--> bloquer/ignorer certains signaux temporairement dans les child
	
	-	textes d'erreur a preciser

	-	free - gestion memoire (voir process avec top?, valgrind?)


-	-	expander/get_env.c -> handle_dollar voir normi
		parser/heredoc.c -> handle_heredocs voir normi


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
