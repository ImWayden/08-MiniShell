/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 22:56:43 by wayden            #+#    #+#             */
/*   Updated: 2023/12/27 05:39:35 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_delone(t_token *node)
{
	if (node)
	{
		(void)node;
	}
}

static t_token	*token_last(t_token *token)
{
	while (token && token->next)
		token = token->next;
	return (token);
}

void	token_add_back(t_token **token, t_token *new)
{
	t_token	*prev;

	if (!*token)
	{
		*token = new;
		return ;
	}
	if (*token && token && new)
	{
		prev = token_last(*token);
		prev->next = new;
	}
}

t_token	*token_new(char *str, int i, int k, t_token_type token)
{
	t_token	*new_token;

	new_token = (t_token *)p_malloc(sizeof(t_token), 1);
	new_token->content = ft_substr_gc(str, k, i - k, 1);
	new_token->size = i - k;
	new_token->type = token;
	new_token->next = NULL;
	return (new_token);
}

bool	is_quote(int *index, char *str, t_token_type *token)
{
	int		i;
	char	delimiter;

	delimiter = str[*index];
	i = *index + 1;
	while (str[i] && str[i] != delimiter)
		i++;
	if (str[i])
	{
		if (delimiter == '\'')
			return (*token = TK_SQUOTE, *index = i, TRUE);
		if (delimiter == '\"')
			return (*token = TK_DQUOTE, *index = i, TRUE);
	}
	else
		return (FALSE);
	return (FALSE);
}

/*
** functions debug only
*/
// const char *token_type_to_string(t_token_type type)
// {
// 	switch (type)
// 	{
// 	case TK_HEREDOC:
// 		return "TK_HEREDOC";
// 	case TK_CONCAT:
// 		return "TK_CONCAT";
// 	case TK_REDIR_ENT:
// 		return "TK_REDIR_ENT";
// 	case TK_REDIR_EXT:
// 		return "TK_REDIR_EXT";
// 	case TK_SPACE:
// 		return "TK_SPACE";
// 	case TK_PIPE:
// 		return "TK_PIPE";
// 	case TK_SQUOTE:
// 		return "TK_SQUOTE";
// 	case TK_DQUOTE:
// 		return "TK_DQUOTE";
// 	case TK_NOTOKEN:
// 		return "TK_NOTOKEN";
// 	case TK_WORD:
// 		return "TK_WORD";
// 	default:
// 		return "Unknown";
// 	}
// }

// // Fonction pour afficher une liste chaînée de 
// //tokens avec des noms de token en chaîne
// void display_token_list(t_token *token_list)
// {
// 	t_token	*current = token_list;

// 	while (current)
// 	{
// 		printf("Content: %s, Size: %ld, Token Type: %s\n",
// current->content, current->size, token_type_to_string(current->type));
// 		current = current->next;
// 	}
// }
