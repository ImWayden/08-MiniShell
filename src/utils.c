/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:20:19 by wayden            #+#    #+#             */
/*   Updated: 2023/11/07 04:20:00 by wayden           ###   ########.fr       */
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
}