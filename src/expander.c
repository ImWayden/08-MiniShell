/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 02:57:50 by wayden            #+#    #+#             */
/*   Updated: 2023/11/13 02:58:00 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void expand_exitcode(int *index, t_token *token)
{
	t_concat s;
	int i;
	
	i = *index;
	s.str = token->content;
	s.part1 = ft_substr(s.str, 0, i);
	i++;
	s.part3 = ft_substr(s.str, i+1, ft_strlen(s.str) - i + 1);
	s.part2 = ft_itoa(*sget_exitcode());
	s.tmp = ft_strjoin(s.part1, s.part2);
	free(token->content);
	token->content = ft_strjoin(s.tmp, s.part3);
	free(s.tmp);
	free(s.part1);
	free(s.part2);
	free(s.part3);
}

void expand(t_token *token)
{
	int k;
	int i;

	i = 0;
	while (token->content[i])
	{
		if (token->content[i] == '$' && token->content[i + 1])
		{
			if (ft_isalnum(token->content[i + 1]))
				expand_var(&i, token);
			else if (token->content[i + 1] == '?')
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
		if (token->type == TK_WORD || token->type == TK_DQUOTE)
			expand(token);
		token = token->next;
	}
}