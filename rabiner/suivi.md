
===============================================================================

EN COURS:

	- codes d'erreur:	renvoi le code int last_status des commandes 
		( echo $? )


	- valgrind

	- echo "'salut'"	bash: 'salut'		minishell:	salut



	- modifie:
				var.c:	str_append_free


===============================================================================

Malloc / free

env_utils.c:	set_no_env:	char **new_env		return:

				add_entry:	char *new_entry		

				free_copy_env_2:	new_env[i] (strdup)

				free_copy_env:	char **new_env		return:

				set_env:	char *key		return (set_no_env(key, arg));
											return (free_copy_env(env, key, arg));
											!!!! return (env) !!! pas free

env.c:			init_env:	char **copy	(+strdup)	return:
													!!! return (NULL) !!! pas free
				
execute_utils.c:	initialise_data: t_fork* data->pid		-> depuis main

execute.c:




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
