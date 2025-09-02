	CODES DE RETOUR BUILTIN

Builtin		cas									Exemple							   # $?


echo		- Option valide ou invalide, 
			  affichage sans erreur				echo hello / echo -nnn hi			0

			- Aucun cas ne provoque 
			  une erreur dans bash		


cd			- Répertoire valide					cd /tmp								0
			- Répertoire inexistant				cd /doesnotexist					1
			- Trop d’arguments					cd arg1 arg2						1
			- HOME non défini et cd 
			  sans argument						unset HOME; cd						1


pwd			- Toujours OK sauf erreur 
			  getcwd extrême					pwd									0

export		- Assignation valide				export VAR=val						0
			- Identifiant invalide				export 1VAR=abc						1
			- Mélange valide + invalide			export GOOD=ok 2BAD=ko				1

unset		- Variable existante ou non, 
			  syntaxe valide					unset VAR / unset UNDEFINED			0
			- Identifiant invalide				unset 2BAD							1

env			- Sans argument						env									0
			- Avec argument (non pour minish)	env foo								1

exit		- exit seul							exit (retourne le dernier $?)		précédent
			- exit 42							exit 42								42
			- exit abc (non numérique)			exit abc							2
			- exit 123 abc (trop d’args)		exit 1 2							1 (pas de sortie)
			- exit overflow						exit 999999999999999999999999		2