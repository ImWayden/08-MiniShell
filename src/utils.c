/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:20:19 by wayden            #+#    #+#             */
/*   Updated: 2023/12/27 05:38:23 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_space(char c)
{
	return (c == '\t' || c == '\n' || c == ' ');
}

char	*ft_strncpy(char *s1, char *s2, int n)
{
	int	i;

	i = -1;
	while (++i < n && s2[i])
		s1[i] = s2[i];
	s1[i] = '\0';
	return (s1);
}

char	*var_interpreter(char *str)
{
	t_env	*env;

	env = *sget_env(NULL);
	while (env != NULL && strcmp(str, env->name) != 0)
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
		return (RETURN_EXIT_NUM_ERR);
	return (res);
}

int	get_nb_cmd(t_token **tokens)
{
	t_token	*start;
	int		nb_cmd;

	start = *tokens;
	if (!start)
		return (0);
	nb_cmd = 1;
	while (start)
	{
		if (start->type == TK_PIPE)
			nb_cmd++;
		start = start->next;
	}
	return (nb_cmd);
}
