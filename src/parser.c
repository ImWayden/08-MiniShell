/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:57:15 by wayden            #+#    #+#             */
/*   Updated: 2023/11/11 04:44:42 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void token_merge(t_token *token, t_token *next)
{
	char *str;
	
	
}

void handle_space(t_token **tokens)
{
	t_token *token;

	token = *tokens;
	while (token->next)
	{
		if ((token->token_type <= TK_SQUOTE && token->token_type <= TK_WORD) 
			&& (token->next->token_type <= TK_SQUOTE && token->next->token_type >= TK_WORD))
			token_merge(token, token->next);
		else
			token = token->next;
	}
}

void expand_var(int *index, t_token *token)
{
	t_concat s;
	int i;
	int k;

	i = *index;
	s.str = token->content;
	s.part1 = ft_substr(s.str, 0, i);
	i++;
	k = i;
	while (ft_isalnum(s.str[k]) || s.str[k] == '_')
		k++;
	s.part3 = ft_substr(s.str, k, ft_strlen(s.str) - k);
	if (--k >= i)
	{
		s.tmp = ft_substr(s.str, i, k - i + 1);
		s.part2 = (char *)(find_node_by_name(sget_env(NULL), s.tmp)->content);
		s.part2 = ft_strjoin(s.part1, s.part2);
		free(token->content);
		token->content = ft_strjoin(s.part2, s.part3);
		free(s.tmp);
		free(s.part2);
	}
	free(s.part1);
	free(s.part3);
}


//expand exitcode todo
//___________________________________________________________
/*                                                           *\
**															 **
** // if (s.str[i] == '?')									 **
** // 	s.part2 = ft_strjoin(s.part1, sget_exitcode());      **
**    														 **
**															 **
\*___________________________________________________________*/


void expand(t_token *token)
{
	char *str;
	int k;
	int i;

	str = token->content;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			if (ft_isalnum(str[i + 1]))
				expand_var(&i, token);
			else if (str[i + 1] == '?')
				expand_exitcode(&i, token);
		}
		i++;
	}
}

void expender(t_token **tokens)
{
	t_token *token;

	token = *tokens;
	while (token)
	{
		if (token->token_type == TK_WORD || token->token_type == TK_DQUOTE)
			expand(token);
		token = token->next;
	}
}

void parser(t_cmd *cmd, t_token **tokens)
{
	bool is_first_token;
	t_token *token;

	expender(tokens);
	handle_space(tokens);
	while (token)
	{
		token = token->next;
	}
}

int get_nb_cmd(t_token **tokens)
{
	t_token *start;
	int nb_cmd;

	start = *tokens;
	if (!start)
		return (0);
	nb_cmd = 1;
	while (start)
	{
		if (start->token_type == TK_PIPE)
			nb_cmd++;
		start = start->next;
	}
	return (nb_cmd);
}

t_cmd **sget_cmd_tab()
{
	static t_cmd **cmd;
	size_t i;
	int nb_cmd;

	if (!sget_init(CMD, NOP) && sget_init(CMD, SET))
	{
		i = -1;
		nb_cmd = get_nb_cmd(sget_token()); // traverse les tokens compte le nombre de token_pipe;
		cmd = (t_cmd **)malloc(sizeof(t_cmd *) * nb_cmd);
		while (++i < nb_cmd)
			parser(cmd[i], sget_token());
	}
	return (cmd);
}