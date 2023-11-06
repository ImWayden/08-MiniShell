/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: therodri <therodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 01:44:44 by therodri          #+#    #+#             */
/*   Updated: 2022/12/10 22:42:35 by therodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	Renvoie le dernier élément de la liste.
*/
#include "../../header/libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}
