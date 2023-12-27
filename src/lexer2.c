/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 04:12:46 by wayden            #+#    #+#             */
/*   Updated: 2023/12/27 04:13:09 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_token(int *index, char *str, t_token_type *token, int i)
{
	if (i == TK_DQUOTE || i == TK_SQUOTE)
		return (is_quote(index, str, token));
	else if (i == TK_HEREDOC || i == TK_CONCAT)
		*index = *index + 1;
	*token = i;
	return (TRUE);
}

bool	is_token(t_token **tk_lst, int *start, int *index, t_token_type *token)
{
	t_token	tokens;
	char	*str;
	int		i;

	i = -1;
	str = sget_input(NULL);
	while (++i < 8)
	{
		tokens = sget_tk_spe(i);
		if (!ft_strncmp(tokens.content, &str[*index], tokens.size))
		{
			if (*token == TK_WORD)
			{
				token_add_back(tk_lst, token_new(str, *index, *start, *token));
				*start = *index;
			}
			return (handle_token(index, str, token, i));
		}
	}
	return (FALSE);
}

void	tokenisateur(t_token **tk_lst, char *str)
{
	int				i;
	int				token_start;
	t_token_type	token;

	i = 0;
	token_start = 0;
	token = TK_NOTOKEN;
	while (str[i])
	{
		if (is_token(tk_lst, &token_start, &i, &token))
		{
			token_add_back(tk_lst, token_new(str, i + 1, token_start, token));
			token_start = i + 1;
			token = TK_NOTOKEN;
			i++;
		}
		else
		{
			token = TK_WORD;
			i++;
		}
	}
	if (token == TK_WORD)
		token_add_back(tk_lst, token_new(str, i + 1, token_start, token));
}

void	clean_quote(t_token **tokens)
{
	t_token	*cur;
	char	*tmp;

	cur = *tokens;
	while (cur)
	{
		if (cur->type == TK_SQUOTE || cur->type == TK_DQUOTE)
		{
			tmp = ft_substr_gc(cur->content, 1, ft_strlen(cur->content) - 2, 1);
			cur->content = tmp;
		}
		cur = cur->next;
	}
}

t_token	**sget_token(void)
{
	static t_token	*token_list = NULL;

	if (!sget_init(TOKEN, NOP) && sget_init(TOKEN, SET))
	{
		token_list = NULL;
		tokenisateur(&token_list, sget_input(NULL));
	}
	return (&token_list);
}
