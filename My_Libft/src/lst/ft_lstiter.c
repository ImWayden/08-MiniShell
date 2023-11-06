/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: therodri <therodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 00:04:51 by therodri          #+#    #+#             */
/*   Updated: 2022/12/10 22:42:31 by therodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Itère sur la liste ’lst’ et applique la fonction
’f’ au contenu chaque élément.
*/
#include "../../header/libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
