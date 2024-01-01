/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_verifier.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 00:39:34 by wayden            #+#    #+#             */
/*   Updated: 2024/01/01 21:55:40 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_builtin(t_cmd *cmd)
{
	char	*name;
	int		exec;

	if (!cmd->cmd)
		return ;
	name = cmd->cmd;
	if (cmd->nb_cmd > 1)
		exec = BUILTINS_NOT_EXEC;
	else
		exec = BUILTINS_EXEC_BACK;
	if (!strcmp(name, "cd"))
		cmd->is_builtin = BUILTINS_CD | exec;
	else if (!strcmp(name, "pwd"))
		cmd->is_builtin = BUILTINS_EXEC | BUILTINS_PWD;
	else if (!strcmp(name, "echo"))
		cmd->is_builtin = BUILTINS_EXEC | BUILTINS_ECHO;
	else if (!strcmp(name, "env"))
		cmd->is_builtin = BUILTINS_EXEC | BUILTINS_ENV;
	else if (!strcmp(name, "exit"))
		cmd->is_builtin = BUILTINS_EXIT | exec;
	else if (!strcmp(name, "export"))
		cmd->is_builtin = BUILTINS_EXPORT | exec;
	else if (!strcmp(name, "unset"))
		cmd->is_builtin = BUILTINS_UNSET | exec;
}

char	*determine_type(t_cmd *cmd, char *path)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
			cmd->type = TYPE_DIR;
		else if ((file_stat.st_mode & S_IXUSR) != 0)
			cmd->type = TYPE_EXEC;
		else
			cmd->type = TYPE_UNKNOWN;
	}
	else
		printf("wth bruh\n");
	return (path);
}

char	*verify(t_cmd *cmd, char *path, int i, int k)
{
	char		*tmp;
	char		*tmp2;

	cmd->type = TYPE_NOTCMD;
	if (!cmd->cmd)
		cmd->type = TYPE_NOCMD;
	if (cmd->cmd && !cmd->cmd[0])
		cmd->type = TYPE_NOTCMD;
	if (access(cmd->cmd, X_OK) == 0)
		return (determine_type(cmd, cmd->cmd));
	while (cmd->cmd && cmd->cmd[0] && path && path[i])
	{
		k = i;
		while (path && path[i] && path[i] != ':')
			i++;
		if (i != k)
		{
			tmp = ft_substr_gc(path, k, i - k, 1);
			tmp2 = ft_strjoin_gc(tmp, "/", 1);
			tmp = ft_strjoin_gc(tmp2, cmd->cmd, 1);
			if (access(tmp, X_OK) == 0)
				return (determine_type(cmd, tmp));
			if (path[i])
				i++;
		}
	}
	return (cmd->cmd);
}

void	verify_commands(t_cmd *cmds)
{
	int		i;
	int		nb_cmd;

	i = 0;
	nb_cmd = cmds->nb_cmd;
	while (i < nb_cmd)
	{
		is_builtin(&cmds[i]);
		if (!cmds[i].is_builtin)
			cmds[i].cmd = verify(&cmds[i], \
				p_find_node_by_name(sget_env(NULL), "PATH"), 0, 0);
		else
		{
			if (cmds[i].is_builtin & BUILTINS_EXEC_BACK)
				*sget_g_exit() = RETURN_EXECBACK;
			cmds[i].type = TYPE_BUILTIN;
		}
		i++;
	}
}
