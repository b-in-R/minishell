/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:11:41 by albertooutu       #+#    #+#             */
/*   Updated: 2025/09/09 13:47:58 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
* Parser and process_token:
* Read the list of t_tokens to build structured t_cmd elements
* (complete commands to execute, every time we find a PIPE
* we create a t_cmd to execute)
*
* "Step" 						   "What do we do?"
* - Start current == NULL, 		   so we create an empty t_cmd
* - As long as we read a token,    we update the corresponding current field
* - As soon as we encounter a PIPE,we reset current = NULL
* - At the end of each loop, 	   we move to next token(tokens = tokens->next)
*
*/
/*
* command: echo hello > out.txt | grep test < file.txt
* list tokens:
*	[WORD: echo] → [WORD: hello] → [REDIR_OUT: >] → [WORD: out.txt] → [PIPE: |]
*	[WORD: grep] → [WORD: test] → [REDIR_IN: <] → [WORD: file.txt]
* Final result: Two t_cmd commands
* 		- command 1: args = {"echo", "hello"} → outfile = "out.txt"→append = 0
* 		- command 2: args = {"grep", "test"} → infile = "file.txt"
*
*/
int	process_token(t_token **tokens, t_cmd **current)
{
	char	*clean;

	if ((*tokens)->type == WORD)
	{
		clean = remove_outer_quotes((*tokens)->value);
		add_arg(&(*current)->args, clean);// -> malloc apres(current->args)
		free(clean);
		*tokens = (*tokens)->next;
	}
	else if ((*tokens)->type == REDIR_IN || (*tokens)->type == REDIR_OUT
		|| (*tokens)->type == REDIR_APPEND || (*tokens)->type == HEREDOC)
	{
		if ((*tokens)->next == NULL || (*tokens)->next->type != WORD)
			return (printf("Syntax error:no filename after redirection\n"), 0);
		handle_redirections(*current, *tokens);
		*tokens = (*tokens)->next->next;
	}
	else if ((*tokens)->type == PIPE)
	{
		*current = NULL;
		*tokens = (*tokens)->next;
	}
	else
		*tokens = (*tokens)->next;
	return (1);
}

t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*current;// initialisation

	cmds = NULL;
	current = NULL;
	while (tokens)
	{
		if (current == NULL)
		{
			current = create_cmd();
			add_cmd(&cmds, current);
		}
		if (!process_token(&tokens, &current))// -> malloc (current->args)
			return (free_cmds(cmds), NULL);
	}
//	free_allocs(current->args);
	return (cmds);
}

/*
*	res = ft_substr(str, 1, len - 2); // supprime les quotes autour
*/
char	*remove_outer_quotes(const char *str)
{
	size_t	len;
	char	*res;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if ((str[0] == '\'' && str[len - 1] == '\'')
		|| (str[0] == '"' && str[len - 1] == '"'))
	{
		res = ft_substr(str, 1, len - 2);
	}
	else
		res = ft_strdup(str);
	return (res);
}

/*
*	Updates the current command's redirection fields based on the token type.
*/
void	handle_redirections(t_cmd *current, t_token *tokens)
{
	if (tokens->type == REDIR_IN)
		current->infile = ft_strdup(tokens->next->value);
	else if (tokens->type == REDIR_OUT)
	{
		current->outfile = ft_strdup(tokens->next->value);
		current->append = 0;
	}
	else if (tokens->type == REDIR_APPEND)
	{
		current->outfile = ft_strdup(tokens->next->value);
		current->append = 1;
	}
	else if (tokens->type == HEREDOC)
	{
		current->heredoc = 1;
		current->delimiter = remove_outer_quotes(tokens->next->value);
	}
}
