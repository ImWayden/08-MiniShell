/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_verifier.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 00:39:34 by wayden            #+#    #+#             */
/*   Updated: 2023/11/18 01:47:51 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool verify(char *cmd, char *path, int i, int k)
{
	char *tmp;
	char *tmp2;

	if (access(cmd, X_OK))
		return (TRUE);
	while (path[i])
	{
		k = i;
		while (path[i] && path[i] != ':')
			i++;
		if (i != k)
		{
			tmp = ft_substr(path, k, i - k);
			tmp2 = ft_strjoin(tmp, "/");
			free(tmp);
			tmp = ft_strjoin(tmp2, cmd);
			free(tmp2);
			if (access(tmp, X_OK) == 0)
				return (free(cmd), cmd = tmp, TRUE);
			i++;
		}
	}
	return (FALSE);
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
		if (!cmds[i].cmd || verifier(cmds[i].cmd, 
				p_find_node_by_name(sget_env(NOP), "PATH"), 0, 0))
			handle_error(ERR_MSG_CMD_NOT, cmds[i].cmd, ERR_CMD_NOT);
		i++;
	}
}