/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:09:18 by albertooutu       #+#    #+#             */
/*   Updated: 2025/06/13 16:06:39 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t	g_signal = 0; // Variable globale pour stocker le status de sortie

/*
*	Quand tu tapes Ctrl-C dans un shell Bash interactif :
*		Le curseur saute à la ligne suivante, retour à la ligne
*		Une ligne vide s’affiche
*		Le prompt (minishell$) se réaffiche
*		Mais rien ne s’exécute (la commande partiellement tapée est ignorée)
*/
void	sigint_handler(int sig)
{
	(void)sig; // On ignore la valeur de `sig`, mais il faut quand même l’argument pour respecter la signature du handler
	g_signal = 1; // On signale qu'on a reçu un SIGINT (pour d'autres parties du programme, si besoin)
	write(1, "\n", 1); // On saute une ligne proprement (comme Bash le fait)
	rl_on_new_line(); // fonction builtin, readline se prépare à afficher un nouveau prompt
	rl_replace_line("", 0); // readline efface la ligne en cours (l’utilisateur a peut-être tapé une commande partielle)
	rl_redisplay(); // readline réaffiche le prompt (proprement, sans `^C`)
}

/*
	Comportement dans un shell classique (bash/zsh) :
		Quand on tapes Ctrl-\, ça envoie le signal SIGQUIT (signal 3).
		Bash, par défaut :
			ne réagit pas dans le shell principal
			affiche rien
	Et surtout : n’interrompt pas le shell interactif
	*/
void	sigquit_handler(int sig)
{
	(void)sig; // On ne fait rien, on ignore le signal SIGQUIT
}

/*Gère les signaux : Ctrl-C et Ctrl-\. Ctrl-D (via readline dans le main)
* envoi le signal SIGINT (Ctrl-C) et appelle sigint_handler qui
* envoie le signal SIGQUIT/ Ctrl-\, et appelle sigquit_handler qui ignore le signal, comme dans bash/zsh
*/
void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
}
