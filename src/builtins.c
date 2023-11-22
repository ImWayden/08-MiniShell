/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:15:36 by wayden            #+#    #+#             */
/*   Updated: 2023/11/22 22:31:01 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	fonctionne bien a priori.
**	a priori pas besoin de quitter le programme en cour
**
*/
void builtin_env(t_env *env)
{
	while (env != NULL)
	{
		ft_putstr_fd(env->name, STDOUT_FILENO);
		ft_putchar_fd('=', STDOUT_FILENO);
		ft_putstr_fd(env->content, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		env = env->next;
	}
	exit(0);
}

/*
**
** 	fonctionne a priori peut etre beosin de plus de tests
**  ne fonctionne pas dans les commande piped
**
*/
void builtin_export(t_scmd *cmd)
{
	int i;
	int j;
	char *content;
	t_env *env;

	i = -1;
	while (cmd->args && cmd->args[++i])
	{
		cmd->args[i][ft_strlen(cmd->args[i]) - 1] = '\0';
		j = 0;
		while (cmd->args[i][j] && cmd->args[i][j] != '=')
			j++;
		content = ft_substr(cmd->args[i], 0, j);
		env = find_node_by_name(sget_env(NULL), content);
		free(content);
		if (env && cmd->args[i][j])
		{
			content = ft_strdup(&cmd->args[i][j + 1]);
			free(env->content);
			env->content = content;
		}
		else
			env_add_back(sget_env(NULL), env_new(cmd->args[i]));
	}
	refresh_env_tab();
}

/*
**	should work flawlessly
**	just besoin de changer strcmp en ft_strcmp
**	que je n'ai malheureusement pas dans ma libft
*/
void builtin_unset(t_scmd *cmd)
{
	int i;

	i = -1;
	while (cmd->args[++i])
		env_remove_if(sget_env(NULL), cmd->args[i], strcmp);
	refresh_env_tab();
}

/*
** executable si la commande contient des pipes
**
**
*/
void builtin_echo(char **strs, int flag_n)
{
	int i;

	i = 0;
	while (strs && strs[i] != NULL)
	{
		write(STDOUT_FILENO, strs[i], ft_strlen(strs[i]));
		i++;
		if (strs[i] != NULL)
			write(STDOUT_FILENO, " ", 1);
	}
	if (!flag_n)
		write(STDOUT_FILENO, "\n", 1);
}

/*
** a priori non executable
** si la commande contient des pipes
**
*/
void builtin_exit(void)
{
	clean_all();
	exit(0);
}
/*
** builtin pwd executable dans ligne
** de commadne contenant des pipes
**
*/
void builtin_pwd(void)
{
	char *cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	printf("%s\n", cwd);
	free(cwd);
}

/*
**
**
**
*/
void builtin_cd(t_scmd *cmd)
{
	char *path;
	if (cmd->args && cmd->args[0])
		path = cmd->args[0];
	else
		path = NULL;
	if (!path || (path[0] != '/' && path[0] != '.') || cmd->args[1] || (chdir(path) == -1))
		perror("cd");
}