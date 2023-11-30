/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:20:19 by wayden            #+#    #+#             */
/*   Updated: 2023/11/30 05:05:54 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_space(char c)
{
	return(c == '\t' || c == '\n' || c == ' ');
}

char *ft_strncpy(char *s1, char *s2, int n)
{
	int i = -1;

	while (++i < n && s2[i])
		s1[i] = s2[i];
	s1[i] = '\0';
	return (s1);
}

char *var_interpreter(char *str)
{
	t_env *env = *sget_env(NULL);

	while(env != NULL && strcmp(str, env->name) != 0)
		env = env->next;
	if (env)
		return (env->content);
	return (NULL);
}

int	ft_simple_atoi_error(const char *nptr)
{
	int				i;
	unsigned int	res;

	i = 0;
	res = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + nptr[i] - 48;
		i++;
	}
	if (i == 0)
		return(RETURN_EXIT_NUM_ERR);
	return (res);
}
