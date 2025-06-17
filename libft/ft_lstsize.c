/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabiner <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:25:35 by rabiner           #+#    #+#             */
/*   Updated: 2024/10/28 15:25:38 by rabiner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
/*
int	main(void)
{
	t_list	*node_a;
	t_list	*node_b;
	char	a[] = "salut";
	char	b[] = "test";

	node_a = ft_lstnew(&a);
	node_b = ft_lstnew(&b);
	printf("size node_a: %i\n", ft_lstsize(node_a));
	printf("size node_b: %i\n", ft_lstsize(node_b));
	return 0;
}
*/
