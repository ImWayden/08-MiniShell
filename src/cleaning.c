/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:24:36 by wayden            #+#    #+#             */
/*   Updated: 2023/12/28 02:40:32 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_scmd(void)
{
	t_scmd	*cmd;

	sget_init(SCMD, SET);
	cmd = sget_scmd(NULL);
	if (!cmd)
		return ;
}

void	clean_env(void)
{
	t_env	*env;
	t_env	*temp;

	env = *sget_env(NULL);
	while (env != NULL)
	{
		temp = env;
		env = env->next;
		env_delone(temp);
	}
}

void	clean_tokens(void)
{
	t_token	*token;
	t_token	*temp;

	token = *sget_token();
	while (token != NULL)
	{
		temp = token;
		token = token->next;
		token_delone(temp);
	}
	sget_init(TOKEN, REFRESH);
}

void	clean_cmds(void)
{
	t_cmd	*cmds;
	int		i;

	i = 0;
	cmds = sget_cmd_tab();
	while (cmds && cmds->nb_cmd > i)
	{
		i++;
	}
	sget_init(CMD, REFRESH);
}

void	clean_all(void)
{
	char	*input;

	input = sget_input(NULL);
	if (input && *input)
		free(input);
	clean_env();
	clean_scmd();
}
