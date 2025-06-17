
Passer en bash	:	bash --norc
verif			:	ps -p $$
valeur exit		:	echo $?

affiche toutes les variables exportees et ayant une valeur	:	env
affiche toutes les variables exportees, meme sans valeur	:	export
creer une variables locale au shell							:	var=val
rend la var visible a l'env									:	export var
creer ou maj + export										:	export var=val
supprime variable											:	unset var
voir si une var specifique existe							:	echo $VAR


Commandes de test:

	echo bonjour ; |						result =	bash: erreur de syntaxe près du symbole inattendu « | »
	echo bonjour | |						result =	bash: erreur de syntaxe près du symbole inattendu « | »
	|										result =	bash: erreur de syntaxe près du symbole inattendu « | »
	echo bonjour |;							result =	bash: erreur de syntaxe près du symbole inattendu « ; »
	echo bonjour ; ls						result =	bonjour
														(resultats ls)
	echo bonjour > test\ 1					result =	creer un fichier 'test 1', "bonjour" dedans
	cd $HOME/Documents						result =	normal
	echo "\s" & echo "\s"					result =	[1] 157267
														\s
														bash-5.2$ \s
														ls (<-- commande complementaire)
														alberto        headers	Makefile   rabiner    srcs   test1.c
														cmds_tests.md  libft	minishell  README.md  suivi
														[1]+  Fini                    echo "\s"

	echo >									result =	bash: erreur de syntaxe pres du symbole inattendu << newline>>
	echo -n -n -nnnn -nnnnm					result =	-nnnnmbash-5.2$ (nouvelle ligne d'entree)
	cat /dev/random | head -n 1	| cat -e	result =	M-^BvPM-:M-HM-\^MM-fV0M-P^WM-^@H$ (dev/random: donnees binaires 
															aleatoires, head -n 1: lit 1 ligne jusqu'a \n,
															cat -e: affiche caracteres invisibles)
	unset var1 # with undefined	var1		result =	(rien)
	export "" et unset ""					result =	export: not valid in this context:
														unset: : invalid parameter name
	echo test > file test1					result =	creer fichier file, "test test1" dedans
	$										result =	bash: $ : commande introuvable
	not_cmd bonjour > salut					result =	bash: not_cmd : commande introuvable
	env puis export puis env # 				result = 	?? vars aren't sorted
	cat Makefile | grep pr | head -n 5 | cd libft	 # check status code
											result =	exit= 1
														cd: aucun fichier ou dossier de ce nom: libft
														cat: Makefile: Aucun fichier ou dossier de ce nom
	cat Makefile | grep pr | head -n 5 | cat test (bin) # check status code
											result =	exit= 1
														cat: Makefile: Aucun fichier ou dossier de ce nom
														cat: test: Aucun fichier ou dossier de ce nom
	cat Makefile | grep pr | head -n 5 | hello (NA) # check status code
											result =	cat: Makefile: Aucun fichier ou dossier de ce nom
														zsh: command not found: hello
	echo bonjour >>> test					result =	erreur de syntaxe pres du symbole inattendu " > "
	echo bonjour > > out					result =	erreur de syntaxe pres du symbole inattendu " > "
	echo 2 >> out1 > out2					result =	creer fichiers out1 et out2, out2: "2" dedans
	echo 2 > out1 >> out2					result =	creer fichiers out1 et out2, out2: "2" dedans
	cat < test # with non-existent test		result =	bash: test: Aucun fichier ou dossier de ce nom
	export var; export var=test				result =	
	echo bonjour > $test # with	test not defined
											result =	bash: $test : redirection ambigue
	file_name_in_current_dir				result =	bash: file_name_in_current_dir : commande introuvable
	cd ../../../../../.. ; pwd				result =	/
	ctrl-C . 130 sur bin(ex : sleep 10)&line vide
											result =	exit= 130
	ctrl-\ .131 sur bin						result =	exit= 131
	echo "bip | bip ; coyotte >	< " "		result =	>
	echo "bip | bip ; coyotte >	< " ""		result =	bip | bip ; coyotte > <
	cat | cat | cat | ls # checkoutputs order
											result =	resultat de ls
														nouvelle commande a entrer
														nouvelle commande a entrer
														nouvelle commande a entrer (les commandes supp ne font rien)
	$bla # with bla not defined				result =	rien
	export var ="cat Makefile |	grep >"		result =	(vide)
														zsh: cat Makefile | grep > not found
	export "test=ici"=coucou				result =	exit= 0 (aucune sortie)
	c$var Makefile # with var=at			result =	si var=at -> cat Makefile
	$LESS$VAR	# if LESS=--  & VAR=color	result =	bash: --color : commande introuvable
	/bin/echo bonjour						result =	bonjour
	not_cmd									result =	bash: not_cmd : commande introuvable
	sleep 5 | exit							result =	sense sortir de minishell? perso pas de retour zsh depuis bash
	echo bonjour > $test w/ t				result =	bash: $test : redirection ambigue
	"exit retour a la ligne"				result =	bash: exit retour a la ligne : commande introuvable
	minishell # binary not in path without "./" before
											result =	si le binaire n'est pas dans PATH: minishell: command not found
														(verifie que minishell ne cherche pas les bianires seulement
														dans ./)
	cat diufosgid # check exit code			result =	exit= 1
	exit # should return the last exit code value
											result =	exit= 0
	exit -10								result =	exit= 246
	exit +10								result =	exit= 10
	;										result =	bash: erreur de syntaxe pres du symbole inattendu " ; "
	echo coucou | ;							result =	bash: erreur de syntaxe pres du symbole inattendu " ; "
	echo "$HOME"							result =	/home/user
	echo '$HOME'							result =	$HOME
	export ; env 	# display is different for both commands
											result =	export affiche les variables exportees,
														env affiche les variables d'env qui ont une valeur	
	echo $HOME								result =	/home/user
	> log echo coucou						result =	creer fichier log, "coucou" dedans
	echo hudifg d | | hugdfihd				result =	bash: erreur de syntaxe pres du symbole inattendu " | "
	echo									result =	(ligne vide)
	echo -n simple							result =	simplebash-5.2$ (nouvelle ligne d'entree)
	echo ''									result =	(ligne vide)
	echo ""									result =	(ligne vide)
	echo "\"								result =	>
	echo "\n \n \n"							result =	\n \n \n
	echo "\n \n \\n"						result =	\n \n \n
	echo ;;									result =	bash: erreur de syntaxe pres du symbole inattendu " ;; "
	echo hi";" hihi							result =	hi; hihi
	echo hi " ; " hihi						result =	hi  ;  hihi
	cd										result =	normal
	cd .									result =	normal
	cd ~									result =	normal
	cd /									result =	normal
	cd no_file								result =	bash: cd: nofile: aucun fichier ou dossier de ce nom
	cd a b c d								result =	bash: cd: trop d'arguments
	pwd a									result =	/home/user/directory
	pwd a b c d								result =	/home/user/directory
	export LOL=lala ROR=rara				result =	definit les 2 variables et assigne une valeur	
	unset LOL ROR							result =	supprime les 2 variables (echo $LOL $ROR)
	export "HI= hi"							result =	normal	
	export "HI =hi"							result =	bash: export: " HI =hi " : identifiant non valable	
	/bin/ls									result =	ls normal
	# write something the press	ctrl+c		result =	someting^C, et ligne suivante
	# write something then press ctrl+d		result =	rien (ding)
	# write something then press ctrl+\		result =	rien
	echo $?									result =	affiche code d'erreur
	l^Ds									result =	bash: l^Ds : commande introuvable
	echo |									result =	>
	| echo									result =	bash: erreur de syntaxe pres du symbole inattendu " \ "
	sort | ls # check output order			result =	boucle infinie (sort), ls
	cat < >									result =	bash: erreur de syntaxe pres du symbole inattendu " > "
	cat < <									result =	bash: erreur de syntaxe pres du symbole inattendu " < "
	cat > >									result =	bash: erreur de syntaxe pres du symbole inattendu " > "
	> a ls > b < Makefile					result =	creer fichiers "a" et "b", a: vide, b: result ls
	echo > a Hello World!					result =	creer fichier "a", "Hello World!" dedans
	> a echo Hello World!					result =	creer fichier "a", "Hello World!" dedans
	cat < Makefile | grep gcc >	output		result =	creer fichier "output", "CC = gcc" dedans (ligne makefile)
	exit 0 | exit 1							result =	exit code: 1
	exit 1 | exit 0							result =	exit code: 0