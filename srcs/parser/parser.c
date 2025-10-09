/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:11:41 by albertooutu       #+#    #+#             */
/*   Updated: 2025/10/09 16:52:50 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	append_to_last_arg(t_pool *pool, t_cmd *cmd, const char *value)
{
	char	**args;
	int		last;
	char	*joined;
	size_t	len_prev;
	size_t	len_new;

	if (!cmd || !cmd->args || !cmd->args[0] || !value)
		return (0);
	args = cmd->args;
	last = 0;
	while (args[last + 1])
		last++;
	len_prev = ft_strlen(args[last]);
	len_new = ft_strlen(value);
	joined = pool_alloc(pool, len_prev + len_new + 1);
	if (!joined)
		return (0);
	ft_memcpy(joined, args[last], len_prev);
	ft_memcpy(joined + len_prev, value, len_new + 1);
	pool_free_one(pool, args[last]);
	args[last] = joined;
	return (1);
}

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
int	process_token(t_expander *exp, t_token **tokens, t_cmd **current)
{
	if ((*tokens)->type == WORD)
	{
		if ((*tokens)->leading_space == 0 && (*current)->args
			&& (*current)->args[0])
		{
			if (!append_to_last_arg(exp->pool, *current, (*tokens)->value))
				return (0);
		}
		else if (!add_arg(exp->pool, &(*current)->args, (*tokens)->value))
			return (0);
	}
	else if ((*tokens)->type == REDIR_IN || (*tokens)->type == REDIR_OUT
		|| (*tokens)->type == REDIR_APPEND || (*tokens)->type == HEREDOC)
	{
		if (!(*tokens)->next || (*tokens)->next->type != WORD)
			return (printf("Syntax error:no filename after redirection\n"), 0);
		handle_redirections(exp, *current, *tokens);
		*tokens = (*tokens)->next->next;
		return (1);
	}
	else if ((*tokens)->type == PIPE)
		*current = NULL;
	*tokens = (*tokens)->next;
	return (1);
}

t_cmd	*parser(t_token *tokens, t_expander *exp)
{
	t_cmd	*cmds;
	t_cmd	*current;

	cmds = NULL;
	current = NULL;
	while (tokens)
	{
		if (current == NULL)
		{
			current = create_cmd(exp);
			if (!current)
			{
				free_cmds(exp->pool, cmds);
				return (NULL);
			}
			add_cmd(&cmds, current);
		}
		if (!process_token(exp, &tokens, &current))
		{
			free_cmds(exp->pool, cmds);
			return (NULL);
		}
	}
	return (cmds);
}

/*
*	res = ft_substr(str, 1, len - 2); // supprime les quotes autour
*/
char	*remove_outer_quotes(t_pool *pool, const char *str, t_cmd *cmd)
{
	size_t	len;
	char	*res;
	int		double_quote;

	double_quote = 0;
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (pool_strdup(pool, str));
	if (str[0] == '\'' && str[len - 1] == '\'')
		res = pool_substr(pool, str, 1, len - 2);
	else if (str[0] == '"' && str[len - 1] == '"')
	{
		res = pool_substr(pool, str, 1, len - 2);
		double_quote++;
	}
	else
		res = pool_strdup(pool, str);
	cmd->double_quote = double_quote;
	return (res);
}

/*
*	Updates the current command's redirection fields based on the token type.
*/
void	handle_redirections(t_expander *exp, t_cmd *cmd, t_token *tok)
{
	char	*value;

	value = tok->next->value;
	if (tok->type == REDIR_IN)
		handle_redir_in(exp, cmd, value);
	else if (tok->type == REDIR_OUT || tok->type == REDIR_APPEND)
		handle_redir_out(exp, cmd, value, tok->type);
	else if (tok->type == HEREDOC)
		handle_heredoc(exp, cmd, tok);
}
