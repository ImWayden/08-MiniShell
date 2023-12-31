/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 03:57:39 by wayden            #+#    #+#             */
/*   Updated: 2023/12/31 01:47:18 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export_env(char c)
{
	t_env	*env;

	env = *sget_env(NULL);
	while (env != NULL)
	{
		if (env->name[0] == c)
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
		if (env->isok == TRUE && env->name[0] == c)
		{
			ft_putstr_fd(env->name, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putchar_fd('\"', STDOUT_FILENO);
			ft_putstr_fd(env->content, STDOUT_FILENO);
			ft_putchar_fd('\"', STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		if (env->isok == FALSE && env->name[0] == c)
		{
			ft_putstr_fd(env->name, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		env = env->next;
	}
}

/*
**
** 	fonctionne a priori peut etre beosin de plus de tests
**  ne fonctionne pas dans les commande piped
**
*/
static void	export_utils2(char *args, int j, t_env *env)
{
	char	*content;

	content = ft_strdup_gc(&args[j + 1], 0);
	env->content = content;
	env->full = ft_strdup_gc(args, 0);
}

static void	export_utils1(char *args, int j, t_env *env)
{
	char	*content;

	while (args[j] && args[j] != '=')
		j++;
	content = ft_substr_gc(args, 0, j, 0);
	env = find_node_by_name(sget_env(NULL), content);
	if (env && args[j])
		export_utils2(args, j, env);
	else if (ft_isalpha(args[0]))
		env_add_back(sget_env(NULL), env_new(args));
	else
	{
		ft_putstr_fd("bash: export: not a valid identifier\n", STDERR_FILENO);
		*sget_exitcode() = 1;
	}
}

void	builtin_export(t_scmd *cmd)
{
	int		i;
	t_env	*env;
	char	c;

	i = -1;
	env = NULL;
	while (cmd->args && cmd->args[++i])
		export_utils1(cmd->args[i], 0, env);
	if (!cmd->args)
	{
		c = 'A';
		while (c <= 'Z')
		{
			print_export_env(c);
			c++;
		}
		c = 'a';
		while (c <= 'z')
		{
			print_export_env(c);
			c++;
		}
	}
	refresh_env_tab();
	*sget_exitcode() = 0;
}

/*
**	should work flawlessly	
**	just besoin de changer strcmp en ft_strcmp
**	que je n'ai malheureusement pas dans ma libft
*/
void	builtin_unset(t_scmd *cmd)
{
	int	i;

	i = -1;
	while (cmd->args && cmd->args[++i])
	{
		if (!ft_isalpha(cmd->args[i][0]))
		{
			printf("\"%s\" is not a valid identifier\n", cmd->args[i]);
			*sget_exitcode() = 1;
		}
		else
			env_remove_if(sget_env(NULL), cmd->args[i], ft_strcmp);
	}
	refresh_env_tab();

}
