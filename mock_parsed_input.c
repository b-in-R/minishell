/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_parsed_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:29:19 by albertooutu       #+#    #+#             */
/*   Updated: 2025/05/23 14:43:54 by albertooutu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/* A tester par rabiner avec les t_cmd simulés ci desous pendant que je develop le parsing:
	- Exécution basique d'une commande (ls, echo, pwd, etc.)
	- Redirections (>, >>, <) avec outfile, append, etc.
	- Enchaînements de commandes via next (execution avec un pipe)
	- Gestion de execve(), fork(), dup2(), etc.*/

// Ex: "ls -l > out.txt"
t_cmd *mock_input(void) {
    t_cmd *cmd = malloc(sizeof(t_cmd));

    cmd->args = malloc(sizeof(char *) * 3);
    cmd->args[0] = strdup("ls");
    cmd->args[1] = strdup("-l");
    cmd->args[2] = NULL;

    cmd->infile = NULL;
    cmd->outfile = strdup("out.txt");
    cmd->append = 0;
    cmd->heredoc = 0;
    cmd->delimiter = NULL;
    cmd->next = NULL;

    return cmd;
}

// Ex: "ls -l | grep minishell"
t_cmd *mock_cmd_pipe(void) {
    t_cmd *cmd1 = malloc(sizeof(t_cmd));
    t_cmd *cmd2 = malloc(sizeof(t_cmd));

    // Première commande : ls -l
    cmd1->args = malloc(sizeof(char *) * 3);
    cmd1->args[0] = strdup("ls");
    cmd1->args[1] = strdup("-l");
    cmd1->args[2] = NULL;
    cmd1->infile = NULL;
    cmd1->outfile = NULL;
    cmd1->append = 0;
    cmd1->heredoc = 0;
    cmd1->delimiter = NULL;
    cmd1->next = cmd2;

    // Deuxième commande : grep minishell
    cmd2->args = malloc(sizeof(char *) * 3);
    cmd2->args[0] = strdup("grep");
    cmd2->args[1] = strdup("minishell");
    cmd2->args[2] = NULL;
    cmd2->infile = NULL;
    cmd2->outfile = NULL;
    cmd2->append = 0;
    cmd2->heredoc = 0;
    cmd2->delimiter = NULL;
    cmd2->next = NULL;

    return cmd1;
}


/* Comment l’utiliser dans main.c
int main(void) {
    // t_cmd *cmd = mock_input();
    t_cmd *cmd = mock_cmd_pipe();
    execute(cmd);
    return 0;
}
*/
