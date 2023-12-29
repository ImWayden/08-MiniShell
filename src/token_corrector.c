/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_corrector.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 04:31:10 by wayden            #+#    #+#             */
/*   Updated: 2023/12/28 04:14:24 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_merge(t_token *token, t_token *next)
{
	char	*str;

	if (token->type == TK_SPACE)
	{
		str = ft_strdup_gc("", 1);
		token->size = 0;
	}
	else
	{
		str = ft_strjoin_gc(token->content, next->content, 1);
		token->size += next->size;
	}
	token->next = next->next;
	token->type = TK_WORD;
	token_delone(next);
	token->content = str;
}

void	remove_spaces(t_token **tokens)
{
	t_token	*token;
	t_token	*tmp;

	token = *tokens;
	while (token && token->next)
	{
		if (token->next->type == TK_SPACE)
		{
			tmp = token->next->next;
			token_delone(token->next);
			token->next = tmp;
		}
		token = token->next;
	}
}

void	merge_quotes(t_token **tokens)
{
	t_token	*token;

	token = *tokens;
	while (token && token->next)
	{
		if ((token->type >= TK_SQUOTE && token->type <= TK_WORD)
			&& (token->next->type >= TK_SQUOTE && token->next->type <= TK_WORD))
			token_merge(token, token->next);
		else
			token = token->next;
	}
}

void	verify_redir(t_token **token_list)
{
	t_token	*token;

	token = *token_list;
	while (token)
	{
		if (!token->next || token->next->type != TK_WORD)
		{
			if (token->type == TK_CONCAT)
				handle_error(ERR_MSG_CONCAT, NULL, ERR_CONCAT);
			else if (token->type == TK_HEREDOC)
				handle_error(ERR_MSG_HEREDOC, NULL, ERR_HEREDOC);
			else if (token->type == TK_REDIR_ENT)
				handle_error(ERR_MSG_REDIR_IN, NULL, ERR_REDIR_IN);
			else if (token->type == TK_REDIR_EXT)
				handle_error(ERR_MSG_REDIR_OUT, NULL, ERR_REDIR_OUT);
		}
		token = token->next;
	}
}

void	correct_tokenlist(t_token **token_list, int nb_cmd)
{
	clean_quote(token_list);
	expender(token_list);
	merge_quotes(token_list);
	remove_spaces(token_list);
	verify_redir(token_list);
	verify_voidcommands(token_list, nb_cmd);
}
