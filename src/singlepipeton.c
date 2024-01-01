/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singlepipeton.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 22:07:02 by wayden            #+#    #+#             */
/*   Updated: 2024/01/01 22:20:08 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*sget_g_exit(void)
{
	static int	exit = 0;

	return (&exit);
}

static void	pipe_actions(int pipe_fd[2], int action)
{
	int			*exit;

	exit = sget_g_exit();
	if (action & PIPE_CREATE)
		pipe(pipe_fd);
	if (action & PIPE_READ)
		read(pipe_fd[0], exit, sizeof(*exit));
	if (action & PIPE_WRITE)
		write(pipe_fd[1], exit, sizeof(*exit));
	if (action & PIPE_CLOSE)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

int	*sget_pipe_g_exit(int action)
{
	static int	pipe_fd[2];

	pipe_actions(pipe_fd, action);
	return (pipe_fd);
}

int	*sget_pipe_g_exit2(int action)
{
	static int	pipe_fd[2];

	pipe_actions(pipe_fd, action);
	return (pipe_fd);
}
