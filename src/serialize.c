/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 09:59:29 by wayden            #+#    #+#             */
/*   Updated: 2024/01/01 22:19:56 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
** fonction pour debug only
*/
void	afficher_scmd(const t_scmd *cmd)
{
	char	**arg;

	printf("Commande : %s\n", cmd->cmd);
	printf("%d", cmd->is_builtin);
	printf("Arguments : ");
	if (cmd->args != NULL)
	{
		arg = cmd->args;
		while (*arg != NULL)
		{
			printf("%s ", *arg);
			arg++;
		}
	}
	printf("\n");
	printf("Est une commande intégrée : %d\n", cmd->is_builtin);
}

void	serialize(t_cmd *cmd, char *name)
{
	int		file;
	char	*itoa;
	int		i;

	i = 1;
	name = ft_strjoin_gc(ft_strjoin_gc(sget_abspath(), "/tmp/", 0), name, 0);
	file = p_open(name, O_CREAT | O_TRUNC | O_RDWR, 0666);
	itoa = ft_itoa_gc(cmd->is_builtin, 1);
	write(file, cmd->cmd, ft_strlen(cmd->cmd));
	write(file, "\n", 1);
	write(file, itoa, ft_strlen(itoa));
	write(file, "\n", 1);
	while (cmd->args[i] != NULL)
	{
		write(file, cmd->args[i], ft_strlen(cmd->args[i]));
		write(file, "\n", 1);
		i++;
	}
	close(file);
}

static void	win_line(t_scmd *cmd, int file)
{
	char	*line;

	line = get_next_line(file);
	add_garbage(line, 0);
	cmd->is_builtin = ft_atoi(line);
	line = get_next_line(file);
	add_garbage(line, 0);
	while (line)
	{
		cmd->args = insert_args_in_tab(cmd->args, line);
		line = get_next_line(file);
		add_garbage(line, 0);
	}
}

t_scmd	*unserialize(char *name)
{
	t_scmd	*cmd;
	int		file;

	cmd = (t_scmd *)p_malloc(sizeof(t_scmd), 0);
	cmd->args = NULL;
	name = ft_strjoin_gc(ft_strjoin_gc(sget_abspath(), "/tmp/", 0), name, 0);
	file = p_open(name, O_CREAT | O_RDWR, 0666);
	if (file == -1)
		return (NULL);
	cmd->cmd = get_next_line(file);
	add_garbage(cmd->cmd, 0);
	if (!cmd->cmd)
		return (NULL);
	win_line(cmd, file);
	close(file);
	return (cmd);
}

t_scmd	*sget_scmd(char *name)
{
	static t_scmd	*cmd;	

	if (!sget_init(SCMD, NOP) && sget_init(SCMD, SET))
	{
		if (name)
			cmd = unserialize(name);
		else
		{
			name = ft_strdup_gc("cmd", 0);
			cmd = unserialize(name);
		}
		if (!cmd)
			return (NULL);
	}
	return (cmd);
}
