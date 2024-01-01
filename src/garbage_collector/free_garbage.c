/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_garbage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 14:42:39 by olcoste           #+#    #+#             */
/*   Updated: 2024/01/01 21:38:52 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./garbage_collector.h"

void	free_tmp_garbage(void)
{
	t_garbage	*head;

	head = get_head_garbage();
	ft_lstclear(&(head->tmp_lst), &free);
}

void	free_all_garbage(void)
{
	t_garbage	*head;

	head = get_head_garbage();
	sget_pipe_g_exit(PIPE_WRITE | PIPE_CLOSE);
	ft_lstclear(&(head->tmp_lst), &free);
	ft_lstclear(&(head->close_lst), &free);
}

void	*exit_error_gc(void)
{
	handle_error(ERR_MSG_MALLOC, NULL, ERR_MALLOC);
	return (NULL);
}
