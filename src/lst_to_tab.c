/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_to_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 01:05:28 by wayden            #+#    #+#             */
/*   Updated: 2023/12/29 06:10:49 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validity_checker(int is_valid, char *content)
{
	if (!is_valid)
		return (1);
	if (content && content[0])
		return (3);
	return (is_valid);
}

char	**env_to_tab(t_env **lst)
{
	t_env	*elem;
	char	**tab;
	int		i;

	i = 0;
	elem = *lst;
	while (elem)
	{
		i++;
		elem = elem->next;
	}
	tab = (char **)p_malloc(sizeof(char *) * (i + 1), 0);
	if (!tab)
		return (NULL);
	elem = *lst;
	i = 0;
	while (elem)
	{
		tab[i] = elem->full;
		i++;
		elem = elem->next;
	}
	tab[i] = NULL;
	return (tab);
}

char	**sget_env_tab(t_refresh refresh)
{
	static char	**env_tab;
	static bool	is_init = FALSE;

	if (!is_init || refresh)
	{
		env_tab = env_to_tab(sget_env(NULL));
		is_init = TRUE;
	}
	return (env_tab);
}

void	refresh_env_tab(void)
{
	sget_env_tab(REFRESH);
}
