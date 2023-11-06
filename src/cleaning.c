/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:24:36 by wayden            #+#    #+#             */
/*   Updated: 2023/11/05 18:40:17 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void clean_env(void)
{
	t_env *env = *sget_env(NULL);
	t_env *temp;

	while (env != NULL)
	{
		temp = env;
		env = env->next;
		env_delone(temp);
	}
	free(sget_env_tab(NOP));
}