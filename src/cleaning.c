/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:24:36 by wayden            #+#    #+#             */
/*   Updated: 2023/11/24 04:23:33 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void clean_scmd(void)
{
	t_scmd *cmd;
	int i;

	i = -1;
	cmd = sget_scmd();
	if(!cmd)
		return;
	p_free((void **)&cmd->cmd);
	while (cmd->args && cmd->args[++i])
		p_free((void **)&cmd->args[i]);
	p_free((void **)&cmd->args);
	p_free((void **)&cmd);
}

void clean_env(void)
{
	t_env *env;
	t_env *temp;
	char **env_tab;

	env = *sget_env(NULL);
	while (env != NULL)
	{
		temp = env;
		env = env->next;
		env_delone(temp);
	}
	env_tab = sget_env_tab(NOP);
	p_free((void **)&env);
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
	t_cmd *cmds;
	int i;

	i = 0;
	cmds = sget_cmd_tab();
	while (cmds && cmds->nb_cmd > i)
	{
		p_free((void **)&cmds[i].cmd);
		p_free((void **)&cmds[i].args);
		p_free((void **)&cmds[i].here_doc);
		i++;
	}
	p_free((void **)&cmds);
	sget_init(CMD, REFRESH);
}

void clean_all(void)
{
	char *input;

	input = sget_input();
	clean_env();
	clean_scmd();
	// p_free((void **)&input);
}