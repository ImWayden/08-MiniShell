/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 04:07:01 by therodri          #+#    #+#             */
/*   Updated: 2024/01/01 21:38:04 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*deletor;

	if (!*lst || !lst)
		return ;
	while ((*lst))
	{
		deletor = (*lst)->next;
		ft_lstdelone((*lst), del);
		(*lst) = deletor;
	}
}

/*
Supprime et libère la mémoire de l’élément passé en
paramètre, et de tous les éléments qui suivent, à
l’aide de ’del’ et de free(3)
Enfin, le pointeur initial doit être mis à NULL.
*/