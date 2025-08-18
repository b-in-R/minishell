/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:19:42 by rabiner           #+#    #+#             */
/*   Updated: 2025/08/15 13:49:47 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void safe_close(int fd)
{
    if (fd > 2)
        close(fd);
}

static void safe_dup2_close(int src, int dst)
{
    if (src >= 0 && src != dst)
	{
        if (dup2(src, dst) == -1)
		{
            // ici, à toi de décider: error_exit ou laisser continuer
            // perror("dup2"); // ou error_exit(...)
        }
        safe_close(src);
    }
}

// configure stdin/stdout avant execution d'une commande selon pipe ou cmd unique
void set_redirection(char **my_env, t_cmd *cmd, int in_fd, int pipe_fd[2])
{
    int fd_in;
    int flags;
    int fd_out;

    // ---- STDIN ----
    // 1) heredoc prioritaire
    if (cmd->in_fd != -1)
        safe_dup2_close(cmd->in_fd, STDIN_FILENO);
		
    // 2) infile
    else if (cmd->infile)
	{
        fd_in = open(cmd->infile, O_RDONLY);
        if (fd_in < 0)
            error_exit(my_env, "setup_redirections: open infile");
        safe_dup2_close(fd_in, STDIN_FILENO);
    }
    // 3) pipe entrant (venant de la cmd précédente)
    else if (in_fd >= 0 && in_fd != STDIN_FILENO) {
        safe_dup2_close(in_fd, STDIN_FILENO);
    }
    // sinon: garder STDIN tel quel

    // ---- STDOUT ----
    // redirection de sortie (> ou >>)
    if (cmd->outfile)
	{
        flags = O_CREAT | O_WRONLY;
        if (cmd->append) 
			flags |= O_APPEND;
        else
			flags |= O_TRUNC;

        fd_out = open(cmd->outfile, flags, 0644);
        if (fd_out < 0)
            error_exit(my_env, "setup_redirections: open outfile");
        safe_dup2_close(fd_out, STDOUT_FILENO);
    }
    // sinon, si on a un prochain maillon, on envoie vers l’extrémité écriture du pipe
    else if (cmd->next && pipe_fd[1] >= 0)
	{
        safe_dup2_close(pipe_fd[1], STDOUT_FILENO);
        // NB: safe_dup2_close ferme pipe_fd[1] si > 2
        pipe_fd[1] = -1; // marquer comme consommé
    }

    // ---- Nettoyage pipe_fd restants dans l’enfant ----
    // On peut fermer l’autre bout s’il existe encore, pour éviter les fuites.
    if (pipe_fd[0] >= 0) safe_close(pipe_fd[0]);
    if (pipe_fd[1] >= 0) safe_close(pipe_fd[1]);
}
