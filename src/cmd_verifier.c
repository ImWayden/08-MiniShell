/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_verifier.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 00:39:34 by wayden            #+#    #+#             */
/*   Updated: 2023/11/22 12:02:48 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void is_builtin(t_cmd *cmd)
{
	char *name;

	name = cmd->cmd;
	if (!strcmp(name, "cd"))
		cmd->is_builtin = BUILTINS_CD;
	else if (!strcmp(name, "pwd"))
		cmd->is_builtin = BUILTINS_EXEC | BUILTINS_PWD;
	else if (!strcmp(name, "echo"))
		cmd->is_builtin = BUILTINS_EXEC | BUILTINS_ECHO;
	else if (!strcmp(name, "env"))
		cmd->is_builtin = BUILTINS_EXEC | BUILTINS_ENV;
	else if (!strcmp(name, "exit"))
		cmd->is_builtin = BUILTINS_EXIT;
	else if (!strcmp(name, "export"))
		cmd->is_builtin = BUILTINS_EXPORT;
	else if (!strcmp(name, "unset"))
		cmd->is_builtin = BUILTINS_EXPORT;
	if (cmd->nb_cmd > 1)
		cmd->is_builtin = cmd->is_builtin | BUILTINS_NOT_EXEC; // changer la valeur pour indiquer que le builtin ne doit pas etre exec;
	else
		cmd->is_builtin = cmd->is_builtin | BUILTINS_EXEC_BACK;
}

char *verify(t_cmd *cmd, char *path, int i, int k)
{
	char *tmp;
	char *tmp2;

	if (access(cmd->cmd, X_OK) == 0)
		return (cmd->cmd);
	while (path && path[i])
	{
		k = i;
		while (path && path[i] && path[i] != ':')
			i++;
		if (i != k)
		{
			tmp = ft_substr(path, k, i - k);
			tmp2 = ft_strjoin(tmp, "/");
			free(tmp);
			tmp = ft_strjoin(tmp2, cmd->cmd);
			free(tmp2);
			if (access(tmp, X_OK) == 0)
				return (tmp);
			free(tmp);
			if (path[i])
				i++;
		}
	}
	return (ft_strdup(""));
}

int verify_commands(t_cmd *cmds)
{
	int i;
	int nb_cmd;
	char *cmd;

	i = 0;
	nb_cmd = cmds->nb_cmd;
	while (i < nb_cmd)
	{
		cmd = cmds[i].cmd;
		is_builtin(&cmds[i]);
		if(!cmds[i].is_builtin)
			cmds[i].cmd = verify(&cmds[i], p_find_node_by_name(sget_env(NOP), "PATH"), 0, 0);
		if (cmd != cmds[i].cmd)
			free(cmd);
		if (!cmds[i].cmd || !cmds[i].cmd[0])
			handle_error(ERR_MSG_CMD_NOT, cmds[i].cmd, ERR_CMD_NOT);
		i++;
	}
}