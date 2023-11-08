/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_to_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 01:05:28 by wayden            #+#    #+#             */
/*   Updated: 2023/11/05 02:06:59 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **env_to_tab(t_env **lst)
{
	t_env 	*elem;
	char 	**tab;
	int		i;

	i = 0;
	elem = *lst;
	while(elem)
	{
		i++;
		elem = elem->next;
	}
	tab = (char **)malloc(sizeof(char *) * (i + 1));
	if(!tab)
		return(NULL);
	elem = *lst;
	i = 0;
	while(elem)
	{
		tab[i] = elem->full;
		i++;
		elem = elem->next;
	}
	tab[i] = NULL;
	return(tab);
}

char **sget_env_tab(t_refresh refresh)
{
	static char **env_tab;
	static bool	is_init = FALSE;
	
	if(!is_init || refresh)
	{
		env_tab = env_to_tab(sget_env(NULL));
		is_init = TRUE;
	}
	return(env_tab);
}

void refresh_env_tab(void)
{
	char	**env_tab;
	t_env	**env;

	env_tab = sget_env_tab(NOP);
	free(env_tab);
	sget_env_tab(REFRESH);
}