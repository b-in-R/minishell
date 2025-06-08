# minishell


	1:	parse input
	2:	separe en tokens (fork)
	3:	evalue les tokens
	4:	execute les tokens (execvp)

	# Minishell

## Structure

- `srcs/` : code source
- `include/` : headers (.h)
- `libft/` : notre lib perso
- `main.c` : point d'entrée
- `parser/` : avec fichiers .c avec les fonctions de parsing
- `executor/` : avec les fichiers .c pour les exécution des commandes

minishell/
├── libft/
├── include/
│   └── minishell.h
├── srcs/
│   ├── main.c
│   ├── parser/
│   ├── executor/
│   └── utils/
├── Makefile
├── README.md

## à Faire

- [ ] Parsing de la ligne de commande
- [ ] Exécution simple
- [ ] Redirections
- [ ] Pipes
- [ ] Signaux
- [ ] Gestion des erreurs


## Commandes utiles

- passer de zsh a bash (sans les config de profil ou rc):	bash --noprofile --norc 