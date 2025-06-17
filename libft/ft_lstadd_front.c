/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:24:10 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/28 15:24:14 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!new || !lst)
		return ;
	new->next = *lst;
	*lst = new;
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
		node_b = ft_lstnew(&b);
		printf("======== list before ========\n");
		printf("node_a->content :%d\n", *(int *)node_a->content);
		printf("node_b->content :%d\n", *(int *)node_b->content);

		ft_lstadd_front(&node_a, node_b);
		printf("======== list after ========\n");
		printf("node_a->content :%d\n", *(int *)node_a->content);
		printf("node_b->content    :%d\n", *(int *)node_b->content);
	}
	else
	{
		printf("2 arg num");
		return (-1);
	}
	return(0);
}
*/
