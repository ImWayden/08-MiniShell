/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_garbage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 13:14:10 by olcoste           #+#    #+#             */
/*   Updated: 2023/12/26 21:53:20 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./garbage_collector.h"

int	add_tmp_garbage(void *ptr)
{
	t_list	*new_node;
	t_list	**head_tmp;

	if (ptr == NULL)
		return (1);
	new_node = ft_lstnew(ptr);
	if (new_node == NULL)
		return (0);
	head_tmp = get_tmp_garbage();
	ft_lstadd_front(head_tmp, new_node);
	return (1);
}

int	add_close_garbage(void *ptr)
{
	t_list	*new_node;
	t_list	**head_close;

	if (ptr == NULL)
		return (1);
	new_node = ft_lstnew(ptr);
	if (new_node == NULL)
		return (0);
	head_close = get_close_garbage();
	ft_lstadd_front(head_close, new_node);
	return (1);
}

void	add_garbage(void *ptr, bool is_tmp)
{
	if (is_tmp)
	{
		if (!add_tmp_garbage(ptr))
			exit_error_gc();
	}
	else if (!add_close_garbage(ptr))
		exit_error_gc();
}
