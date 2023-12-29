/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 02:57:50 by wayden            #+#    #+#             */
/*   Updated: 2023/12/29 05:47:46 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_var(int *index, char *content)
{
	t_concat	s;
	int			i;
	int			k;
	char		*res;

	i = *index;
	s.str = content;
	s.part1 = ft_substr_gc(s.str, 0, i, 1);
	i++;
	k = i;
	while (ft_isalnum(s.str[k]) || s.str[k] == '_')
		k++;
	s.part3 = ft_substr_gc(s.str, k, ft_strlen(s.str) - k, 1);
	if (--k >= i)
	{
		s.tmp = ft_substr_gc(s.str, i, k - i + 1, 1);
		s.part2 = (char *)(p_find_node_by_name(sget_env(NULL), s.tmp));
		s.part2 = ft_strjoin_gc(s.part1, s.part2, 1);
		res = ft_strjoin_gc(s.part2, s.part3, 0);
		if (res)
			content = res;
	}
	return (content);
}

char	*expand_exitcode(int *index, char *content)
{
	t_concat	s;
	int			i;

	i = *index;
	s.str = content;
	s.part1 = ft_substr_gc(s.str, 0, i, 1);
	i++;
	s.part3 = ft_substr_gc(s.str, i + 1, ft_strlen(s.str) - i + 1, 1);
	if (s.str[i] == '?')
		s.part2 = ft_itoa_gc(*sget_exitcode(), 1);
	else if (s.str[i] == '0')
		s.part2 = ft_strdup_gc("minishell", 1);
	else
		s.part2 = ft_strdup_gc("", 1);
	s.tmp = ft_strjoin_gc(s.part1, s.part2, 1);
	content = ft_strjoin_gc(s.tmp, s.part3, 1);
	return (content);
}

char	*expand(char *content)
{
	int	i;

	i = 0;
	if (!content)
		return (NULL);
	while (content[i])
	{
		if (content[i] == '$' && content[i + 1])
		{
			if (ft_isalpha(content[i + 1]))
				content = expand_var(&i, content);
			else if (ft_isdigit(content[i + 1]) || content[i + 1] == '?')
				content = expand_exitcode(&i, content);
			else
				i++;
		}
		else if (content[i])
			i++;
	}
	return (content);
}

void	expender(t_token **tokens)
{
	t_token	*token;

	token = *tokens;
	while (token)
	{
		if (token->type == TK_WORD || token->type == TK_DQUOTE)
		{
			token->content = expand(token->content);
			if (token->type == TK_WORD && token->next \
				&& (token->next->type == TK_DQUOTE \
					|| token->next->type == TK_SQUOTE) \
				&& token->content \
				&& token->content[ft_strlen(token->content) - 1] == '$')
			{
				token->content = \
					ft_substr_gc(token->content, 0, \
						ft_strlen(token->content) - 1, 1);
			}
			token->type = TK_WORD;
		}
		else if (token->type == TK_SQUOTE)
			token->type = TK_WORD;
		token = token->next;
	}
}
