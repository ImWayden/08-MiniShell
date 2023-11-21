/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_verifier.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 00:39:34 by wayden            #+#    #+#             */
/*   Updated: 2023/11/21 10:57:31 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *verify(t_cmd *cmd, char *path, int i, int k)
{
	char *tmp;
	char *tmp2;

	if (access(cmd->cmd, X_OK) == 0)
		return (cmd->cmd);
	//printf("path[i] = %c path = %s\n", path[i], path); // debug
	while (path && path[i])
	{
		k = i;
		while (path && path[i] && path[i] != ':')
			i++;
		if (i != k)
		{
			tmp = ft_substr(path, k, i - k);
			//printf("%s\n", tmp); //debug
			tmp2 = ft_strjoin(tmp, "/");
			free(tmp);
			tmp = ft_strjoin(tmp2, cmd->cmd);
			free(tmp2);
			if (access(tmp, X_OK) == 0)
				return (tmp);
			free(tmp);
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
		cmds[i].cmd = verify(&cmds[i], p_find_node_by_name(sget_env(NOP), "PATH"), 0, 0);
		if(cmd != cmds[i].cmd)
			free(cmd);
		if (!cmds[i].cmd || !cmds[i].cmd[0])
		{
			printf("omfg\n");
			handle_error(ERR_MSG_CMD_NOT, cmds[i].cmd, ERR_CMD_NOT);
		}
		i++;
	}
}