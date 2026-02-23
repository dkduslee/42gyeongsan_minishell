/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylee <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:54:52 by aylee             #+#    #+#             */
/*   Updated: 2025/04/08 16:54:54 by aylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*remove;

	while (*lst)
	{
		remove = *lst;
		*lst = remove->next;
		ft_lstdelone(remove, del);
	}
	*lst = NULL;
}
