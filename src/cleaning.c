/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:24:36 by wayden            #+#    #+#             */
/*   Updated: 2023/11/15 03:22:51 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void clean_env(void)
{
	t_env	*env;
	t_env	*temp;
	char	**env_tab;
	
	env = *sget_env(NULL);
	env_tab = sget_env_tab(NOP);
	while (env != NULL)
	{
		temp = env;
		env = env->next;
		env_delone(temp);
	}
	p_free((void **)&env_tab);
}

void clean_tokens(void)
{
	t_token *token;
	t_token *temp;

	token = *sget_token();
	while (token != NULL)
	{
		temp = token;
		token = token->next;
		token_delone(temp);
	}
	sget_init(TOKEN, REFRESH);
}

void clean_cmds(void)
{
	t_cmd	*cmds;
	int		i;

	i = 0;
	cmds = sget_cmd_tab();
	while(cmds && cmds->nb_cmd > i)
	{
		p_free((void **)&cmds[i].args);
		i++;
	}
	p_free((void **)&cmds);
}