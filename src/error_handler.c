/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 03:56:12 by wayden            #+#    #+#             */
/*   Updated: 2023/12/31 01:16:59 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	*sget_location_flag(t_error replace)
{
	static t_error	flag = 0;

	if (replace)
		flag = replace;
	return (&flag);
}

char	*manage_location(void)
{
	t_error		flag;
	char		*msg;

	flag = *sget_location_flag(0);
	if (flag & ERR_ENV)
		msg = ERR_MSG_ENV;
	if (flag & ERR_TOKEN)
		msg = ERR_MSG_TOKEN;
	if (flag & ERR_PARSER)
		msg = ERR_MSG_PARSER;
	if (flag & ERR_SCMD)
		msg = ERR_MSG_SCMD;
	return (msg);
}

void	cleanhub(void)
{
	t_error		flag;

	flag = *sget_location_flag(0);
	if (flag & ERR_ENV)
		clean_env();
	if (flag & ERR_TOKEN)
		clean_tokens();
	if (flag & ERR_PARSER)
		clean_cmds();
	if (flag & ERR_SCMD)
		clean_scmd();
}

t_error	change_exitcode(t_error errcode)
{
	if (errcode == ERR_NOCOMMAND)
		return (0);
	else if (errcode == ERR_ACCESS || errcode == ERR_EXIT_NUM)
		return (1);
	else if (errcode == ERR_CD)
		return (1);
	else if (errcode == ERR_ISDIR)
		return (126);
	else if (errcode == ERR_CMD_NOT)
		return (127);
	else
		return (2);
	return (0);
}

void	handle_error(const char *msg, const char *file, t_error errorcode)
{
	char	*from;
	t_error	exitcode;

	from = manage_location();
	exitcode = change_exitcode(errorcode);
	if (*sget_g_exit() == RETURN_EXECBACK)
	{
		ft_putstr_fd("minishell :", STDERR_FILENO);
		if (file && file[0])
			ft_putstr_fd(ft_strjoin_gc((char *)file, " : ", 1), STDERR_FILENO);
		ft_putstr_fd((char *)msg, STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
		*sget_exitcode() = 1;
		return ;
	}
	dup2(STDERR_FILENO, STDOUT_FILENO);
	if (errorcode & ERR_CLOSE || errorcode & ERR_OPEN || errorcode & ERR_ACCESS \
		|| errorcode & ERR_CMD_NOT || errorcode & ERR_ISDIR)
		printf("minishell : %s : %s : %s\n", from, file, msg);
	else if (!(errorcode & ERR_NOCOMMAND))
		printf("minishell : %s : %s\n", from, msg);
	rl_clear_history();
	free_all_garbage();
	(void)((close(STDERR_FILENO), 1) && (close(STDOUT_FILENO), 1));
	exit(exitcode);
}
