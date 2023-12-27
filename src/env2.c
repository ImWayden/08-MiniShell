/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 04:05:40 by wayden            #+#    #+#             */
/*   Updated: 2023/12/28 00:41:26 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	**sget_env(char **envp)
{
	static t_env	*env = NULL;
	static bool		initialized = FALSE;
	int				i;

	if (!initialized)
	{
		i = -1;
		env = NULL;
		while (envp[++i])
			env_add_back(&env, env_new(envp[i]));
		initialized = TRUE;
	}
	return (&env);
}

t_env	*find_node_by_name(t_env **begin_list, const char *name_to_find)
{
	t_env	*current;

	current = *begin_list;
	while (current)
	{
		if (ft_strcmp(current->name, name_to_find) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}
