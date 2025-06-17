
===============================================================================

EN COURS:

	explications et commentaires:

		-	execute.c - execute();


===============================================================================


QUESTIONS - INFOS PARTAGEES:

	- 	est ce que si depuis minishell on fait cd etc, ca se repercute aussi sur
		le shell principal?

	-	variable globale (besoin de char **envp, a mettre dans t_cmd->g_env ?)



===============================================================================

GENERAL:

	-	structure.md:	reecrire plus simplement

	-	toutes les commandes builtin sont executees dans un fork
		--> quand il n'y a pas de pipe (1 seule cmd builtin), executer direct
			dans le parent (actuellement child)

	-	gestion signaux dans child et parent, ne doit pas reagir comme un shell
		en plein readline pendant waitpid
		--> bloquer/ignorer certains signaux temporairement dans les child
	
	-	textes d'erreur a preciser

	-	free - gestion memoire (voir process avec top?, valgrind?)

	-	redirections avec heredoc (t_cmd->heredoc   t_cmd->delimiter)
		--> fonction handle_heredoc


===============================================================================

INFOS A TROUVER:

	-	heredoc


===============================================================================

FONCTIONS:

		general:

			- textes d'erreur
			- free (voir process avec top?)
			- normi

		builtin_1.c:

			ft_pwd:						-  voir malloc 	char	cwd[1024];


		builtin_utils.c:

			set_env:					- normi (25 lignes)
										- explications

			unset_env:					- explications


		check_builtin.c:

			error_exit:					- voir les free avant


		execute.c:

			execute_command:			- texte d'erreur a voir selon bash?

			execute:					- normi (25 lignes)
										- explications


		redirection.c:

			setup_redirections:			- normi (25 lignes)
										- explications


===============================================================================