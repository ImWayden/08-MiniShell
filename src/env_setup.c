/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 21:52:28 by wayden            #+#    #+#             */
/*   Updated: 2023/12/27 05:38:03 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env_delone(t_env *node)
{
	if (node)
	{
		(void)node;
	}
}

void	env_remove_if(t_env **begin_list, char *name, int (*cmp)())
{
	t_env	*current;

	current = *begin_list;
	while (current && current->next)
	{
		if ((*cmp)(current->next->name, name) == 0)
			current->next = current->next->next;
		current = current->next;
	}
	current = *begin_list;
	if (current && (*cmp)(current->name, name) == 0)
	{
		*begin_list = current->next;
	}
}

static t_env	*env_last(t_env *env)
{
	while (env && env->next)
		env = env->next;
	return (env);
}

void	env_add_back(t_env **env, t_env *new)
{
	if (!*env)
	{
		*env = new;
		return ;
	}
	if (*env && env && new)
		env_last(*env)->next = new;
}

t_env	*env_new(char *str)
{
	t_env	*new_var;
	int		i;

	i = 0;
	new_var = (t_env *)p_malloc(sizeof(t_env), 0);
	ft_bzero(new_var, sizeof(t_env));
	if (!new_var)
		return (NULL);
	while (str[i] && str[i] != '=')
		i++;
	new_var->name = (char *)p_malloc(sizeof(char) * (i + 1), 0);
	ft_strncpy(new_var->name, str, i);
	if (str[i])
	{
		new_var->content = ft_strdup_gc(&str[i + 1], 0);
		new_var->isok = TRUE;
	}
	else
		new_var->content = ft_strdup_gc(&str[i + 1], 0);
	new_var->next = NULL;
	new_var->full = ft_strdup_gc(str, 0);
	return (new_var);
}
