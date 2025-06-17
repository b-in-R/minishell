/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:25:26 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/28 15:25:29 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
/*
int	main(int ac, char **av)
{
	int		a;
	int		b;
	t_list	*node_a;
	t_list	*node_b;

	if (ac == 3)
	{
		a = atoi(av[1]);
		b = atoi(av[2]);
		node_a = ft_lstnew(&a);
		printf("node_a->content = %d\n", *(int *)node_a->content);
		node_b = ft_lstnew(&b);
		printf("node_b->content = %d\n", *(int *)node_b->content);
	}
	else
	{
		printf("2 arg numeral (atoi)");
		return (-1);
	}
	return(0);
}
*/
