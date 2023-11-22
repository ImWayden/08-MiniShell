/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 21:52:28 by wayden            #+#    #+#             */
/*   Updated: 2023/11/22 20:14:33 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void env_delone(t_env *node)
{
	if (node)
	{
		p_free((void **)&node->name);
		p_free((void **)&node->content);
		p_free((void **)&node->full);
		p_free((void **)&node);
	}
}

void env_remove_if(t_env **begin_list, char *name, int (*cmp)())
{
	t_env *cur = *begin_list;

	if (begin_list == NULL || *begin_list == NULL)
		return;

	if (!cmp(cur->name, name))
	{
		*begin_list = cur->next;
		env_delone(cur);
		env_remove_if(begin_list, name, cmp);
	}
	cur = *begin_list;
	env_remove_if(&cur->next, name, cmp);
}

static t_env *env_last(t_env *env)
{
	while (env && env->next)
		env = env->next;
	return (env);
}

void env_add_back(t_env **env, t_env *new)
{
	if (!*env)
	{
		*env = new;
		return;
	}
	if (*env && env && new)
		env_last(*env)->next = new;
}

t_env *env_new(char *str)
{
	t_env *new_var;
	int i;

	i = 0;
	new_var = (t_env *)p_malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	while (str[i] && str[i] != '=')
		i++;
	new_var->name = (char *)p_malloc(sizeof(char) * (i + 1));
	ft_strncpy(new_var->name, str, i);
	if (str[i])
		new_var->content = ft_strdup(&str[i + 1]);
	else
		return (env_delone(new_var), NULL);
	new_var->next = NULL;
	new_var->full = ft_strdup(str);
	return (new_var);
}

t_env **sget_env(char **envp)
{
	static t_env *env = NULL;
	static bool initialized = FALSE;
	int i;

	if (!initialized)
	{
		i = -1;
		env = NULL;
		while (envp[++i])
			env_add_back(&env, env_new(envp[i]));
		initialized = TRUE;
	}

	return &env;
}

t_env *find_node_by_name(t_env **beign_list, const char *name_to_find)
{
	t_env *current = *beign_list;

	while (current != NULL)
	{
		if (strcmp(current->name, name_to_find) == 0)
			return current;
		current = current->next;
	}
	return NULL;
}
