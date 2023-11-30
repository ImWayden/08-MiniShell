/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 02:57:50 by wayden            #+#    #+#             */
/*   Updated: 2023/11/30 05:57:03 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char  *expand_var(int *index, char *content)
{
	t_concat s;
	int i;
	int k;

	i = *index;
	s.str = content;
	s.part1 = ft_substr(s.str, 0, i);
	i++;
	k = i;
	while (ft_isalnum(s.str[k]) || s.str[k] == '_')
		k++;
	s.part3 = ft_substr(s.str, k, ft_strlen(s.str) - k);
	if (--k >= i)
	{
		s.tmp = ft_substr(s.str, i, k - i + 1);
		s.part2 = (char *)(p_find_node_by_name(sget_env(NULL), s.tmp));
		s.part2 = ft_strjoin(s.part1, s.part2);
		free(content);
		content = ft_strjoin(s.part2, s.part3);
		free(s.tmp);
		free(s.part2);
	}
	free(s.part1);
	free(s.part3);
	return(content);
}

char *expand_exitcode(int *index, char *content)
{
	t_concat s;
	int i;
	
	i = *index;
	s.str = content;
	s.part1 = ft_substr(s.str, 0, i);
	i++;
	s.part3 = ft_substr(s.str, i+1, ft_strlen(s.str) - i + 1);
	s.part2 = ft_itoa(*sget_exitcode());
	s.tmp = ft_strjoin(s.part1, s.part2);
	free(content);
	content = ft_strjoin(s.tmp, s.part3);
	free(s.tmp);
	free(s.part1);
	free(s.part2);
	free(s.part3);
	return (content);
}

char *expand(char *content)
{
	int i;

	i = 0;
	if(!content)
		return (NULL);
	while (content[i])
	{
		if (content[i] == '$' && content[i + 1])
		{
			if (ft_isalnum(content[i + 1]))
				content = expand_var(&i, content);
			else if (content[i + 1] == '?')
				content = expand_exitcode(&i, content);
		}
		i++;
	}
	return (content);
}

void expender(t_token **tokens)
{
	t_token *token;

	token = *tokens;
	while (token)
	{
		if (token->type == TK_WORD || token->type == TK_DQUOTE)
		{
			token->content = expand(token->content);
			token->type = TK_WORD;
		}
		else if(token->type == TK_SQUOTE)
			token->type = TK_WORD;		
		token = token->next;
	}
}
