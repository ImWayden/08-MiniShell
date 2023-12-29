/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 04:08:15 by wayden            #+#    #+#             */
/*   Updated: 2023/12/29 06:22:18 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	p_close(int fd, char *file)
{
	int	cl;

	cl = close(fd);
	if (cl == -1)
		handle_error(ERR_MSG_CLOSE, file, ERR_CLOSE);
}

void	p_dup2(int fd, int fd2, char *debug)
{
	int	dp;

	dp = dup2(fd, fd2);
	if (dp == -1)
	{
		printf("%s\n", debug);
		handle_error(ERR_MSG_DUP2, NULL, ERR_DUP2);
	}
}

void	builtin_executor(t_cmd *cmd)
{
	if (cmd->is_builtin & BUILTINS_PWD)
		builtin_pwd();
	else if (cmd->is_builtin & BUILTINS_ENV)
		builtin_env(*sget_env(NULL));
	else if (cmd->is_builtin & BUILTINS_ECHO)
		builtin_echo(cmd->args, 0);
	//close_things(cmd);
	cleanhub();
	free_all_garbage();
	exit(0);
}

void	handle_builtins2(char *name, t_cmd *cmds)
{
	t_scmd	*cmd;

	cmd = sget_scmd(name);
	if (cmd->is_builtin & BUILTINS_CD)
		builtin_cd(cmd->args);
	else if (cmd->is_builtin & BUILTINS_EXPORT)
		builtin_export(cmd);
	else if (cmd->is_builtin & BUILTINS_UNSET)
		builtin_unset(cmd);
	else if (cmd->is_builtin & BUILTINS_EXIT)
		builtin_exit(cmd->args);
	//close_things(cmds);
}

void	builtin_handler(t_cmd *cmd, int n)
{
	char	*str;
	char	*itoa;

	if (cmd->is_builtin & BUILTINS_EXEC)
	{
		builtin_executor(cmd);
		free_all_garbage();
		exit(0);
	}
	else if (cmd->is_builtin & BUILTINS_NOT_EXEC)
	{
		itoa = ft_itoa_gc(n, 1);
		str = ft_strjoin_gc("cmd", itoa, 1);
		(void)(serialize(cmd, str));
		handle_builtins2(str, cmd);
		free_all_garbage();
		exit(*sget_exitcode());
	}
	else if (cmd->is_builtin & BUILTINS_EXEC_BACK)
	{
		serialize(cmd, "cmd");
		cleanhub();
		free_all_garbage();
		exit(RETURN_EXECBACK);
	}
}
