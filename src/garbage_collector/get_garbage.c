/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getGarbage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 13:01:42 by olcoste           #+#    #+#             */
/*   Updated: 2023/12/26 14:25:56 by olcoste          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./garbage_collector.h"

t_garbage	*get_head_garbage(void)
{
	static int			first_time = 1;
	static t_garbage	head;

	if (first_time == 1)
	{
		ft_bzero(&head, sizeof(head));
		first_time = 0;
	}
	return (&head);
}

t_list	**get_tmp_garbage(void)
{
	t_garbage	*head;

	head = get_head_garbage();
	return (&(head->tmp_lst));
}

t_list	**get_close_garbage(void)
{
	t_garbage	*head;

	head = get_head_garbage();
	return (&(head->close_lst));
}
