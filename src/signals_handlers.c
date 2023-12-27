/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 04:17:38 by wayden            #+#    #+#             */
/*   Updated: 2023/12/27 05:36:35 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		*sget_exitcode() = RETURN_SIGINT;
	}
	if (signum == SIGQUIT)
	{
		*sget_exitcode() = RETURN_SIGQUIT;
	}
}

void	input_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 2);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
		*sget_exitcode() = 130;
	}
	if (signum == SIGQUIT)
	{
	}
}

void	child_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		clean_all();
		clean_tokens();
		clean_cmds();
		*sget_exitcode() = 130;
		free_all_garbage();
		exit(*sget_exitcode());
	}
	if (signum == SIGQUIT)
	{
		clean_all();
		clean_tokens();
		clean_cmds();
		*sget_exitcode() = 131;
		free_all_garbage();
		exit(*sget_exitcode());
	}
}
