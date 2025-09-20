/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:09:18 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/10 11:11:25 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Global variable to store the exit status
volatile sig_atomic_t	g_signal = 0;

/*
* When you type Ctrl-C in an interactive Bash shell:
* The cursor jumps to the next line, a newline
* ​​A blank line is displayed
* The prompt (minishell$) is redisplayed
* But nothing is executed (the partially typed command is ignored)
*
* We ignore the value of `sig`
* rl_replace_line("", 0); // Readline deletes the current line
* write(1, "\n", 1); // We skip a line cleanly (like Bash does)
* rl_on_new_line(); // readline prepares to display a new prompt
* rl_redisplay(); // Readline redisplays the prompt (cleanly, without `^C`)
*/
void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
* Behavior in a classic shell (bash/zsh):
* When you type Ctrl-\, it sends the SIGQUIT signal (signal 3).
* Bash, by default:
* Doesn't react in the main shell
* Prints nothing
* And most importantly: Doesn't interrupt the interactive shell
* We do nothing, we ignore the SIGQUIT signal
*/
void	sigquit_handler(int sig)
{
	(void)sig;
}

/*
*	When you type Ctrl-C in a heredoc (here-document):
* The cursor jumps to the next line, a newline
* A blank line is displayed
* The prompt (heredoc$) is not redisplayed
* The heredoc is closed (the file descriptor is closed)
* The shell continues to wait for the next command
* We close the standard input (STDIN_FILENO) to stop reading from the heredoc
*/
void	sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

/* Handles signals: Ctrl-C and Ctrl-\. Ctrl-D (via readline in main)
* Sends the SIGINT signal (Ctrl-C) and calls sigint_handler
* Sends the SIGQUIT signal (Ctrl-\), and calls sigquit_handler,
* which ignores the signal
*/
void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
}
