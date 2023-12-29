/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:57:15 by wayden            #+#    #+#             */
/*   Updated: 2023/12/29 06:13:11 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**insert_args_in_tab(char **tab, char *str)
{
	int		size;
	char	**new_array;

	size = 0;
	if (!tab || !*tab)
	{
		new_array = (char **)p_malloc(sizeof(char *) * 2, 0);
		new_array[0] = str;
		new_array[1] = NULL;
	}
	else
	{
		while (tab[size] != NULL)
			size++;
		new_array = (char **)p_malloc(sizeof(char *) * (size + 2), 0);
		new_array[size + 1] = NULL;
		new_array[size] = str;
		while (--size >= 0)
			new_array[size] = tab[size];
	}
	return (new_array);
}

t_token	**parser(t_cmd *cmd, t_token **tokens)
{
	static t_token	*token;

	token = *tokens;
	if (token && token->type == TK_PIPE)
		token = token->next;
	while (token && token->type != TK_PIPE)
	{
		if (token->type == TK_WORD)
			cmd->args = insert_args_in_tab(cmd->args, token->content);
		else
			token = parser_handle_special(token, cmd);
		token = token->next;
	}
	if (cmd->args)
		cmd->cmd = ft_strdup_gc(cmd->args[0], 1);
	else
	{
		free_all_garbage();
		exit(0);
	}
	if (token && token->type == TK_PIPE && !token->next)
		handle_error(ERR_MSG_PIPE, NULL, ERR_PIPE);
	if (token && token->next)
		token = token->next;
	return (&token);
}

void	handle_unclosed_pipe(void)
{
	char	*input;
	char	*s2;
	char	*s1;

	input = NULL;
	input = sget_input(NULL);
	s2 = readline(">");
	s1 = ft_strdup(input);
	clean_tokens();
	input = ft_strjoin(s1, s2);
	(void)((free(s1),1) && (free(s2),1));
	sget_init(INPUT, REFRESH);
	sget_input(input);
	sget_init(TOKEN, REFRESH);
	if (!*sget_token())
	{
		clean_all();
		clean_tokens();
		exit(0);
	}
	verify_voidcommands(sget_token(), get_nb_cmd(sget_token()));
}

t_cmd	*sget_cmd_tab(void)
{
	static t_cmd	*cmd;
	int				i;
	t_token			**token_list;
	int				nb_cmd;

	if (!sget_init(CMD, NOP) && sget_init(CMD, SET))
	{
		i = -1;
		token_list = sget_token();
		if (!*token_list)
			return (NULL);
		nb_cmd = get_nb_cmd(token_list);
		cmd = (t_cmd *)p_malloc(sizeof(t_cmd) * (nb_cmd), 0);
		while (++i < nb_cmd)
		{
			ft_memset(&cmd[i], 0, sizeof(t_cmd));
			cmd[i].nb_cmd = nb_cmd;
			token_list = parser(&cmd[i], token_list);
		}
		cmd[0].first = 1;
		cmd[i - 1].last = 1;
	}
	return (cmd);
}
