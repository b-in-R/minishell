
===============================================================================

general:

	variable globale g_env
	verifier make ok apres implementation de find_command_path


builtin_1.c:

	ft_pwd:						-  voir malloc 	char	cwd[1024];


builtin_utils.c:

	set_env:					- normi (25 lignes)
								- explications

	unset_env:					- explications


check_builtin.c:

	execute_builtin:			- voir si free avant d'exit  si cmd->args[0] = "exit"


execute.c:

	execute_command:			- texte d'erreur a voir selon bash?
						- A FAIRE -> find_command_path


	execute:					- normi (25 lignes)
								- explications


redirection.c:

	setup_redirections:			- normi (25 lignes)
								- explications


===============================================================================

