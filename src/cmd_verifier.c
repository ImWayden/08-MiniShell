/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_verifier.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 00:39:34 by wayden            #+#    #+#             */
/*   Updated: 2023/12/28 00:21:22 by wayden           ###   ########.fr       */
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

char	*verify(t_cmd *cmd, char *path, int i, int k)
{
	char	*tmp;
	char	*tmp2;

	if (access(cmd->cmd, X_OK) == 0)
		return (cmd->cmd);
	while (path && path[i])
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
				return (tmp);
			if (path[i])
				i++;
		}
	}
	return (ft_strdup_gc("", 1));
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
		if (!cmds[i].cmd || !cmds[i].cmd[0])
			cmds[i].found = 0;
		else
			cmds[i].found = 1;
		i++;
	}
}
