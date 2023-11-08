/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 22:56:43 by wayden            #+#    #+#             */
/*   Updated: 2023/11/08 03:17:05 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token *token_last(t_token *token)
{
	while (token && token->next)
		token = token->next;
	return (token);
}

void token_add_back(t_token **token, t_token *new)
{
	t_token *prev;
	if (!*token)
	{
		*token = new;
		return;
	}
	if (*token && token && new)
	{
		prev = token_last(*token);
		prev->next = new;
		new->prev = prev;
	}
}

t_token *token_new(char *str, int i, int k, t_token_type token)
{
	t_token *new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	new_token->content = ft_substr(str, k, i - k);
	new_token->size = i - k;
	new_token->token_type = token;

	new_token->next = NULL;
	return (new_token);
}

bool is_quote(int *index, char *str, t_token_type *token)
{
	int i;
	char delimiter;

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
		return FALSE;
}

bool handle_token(int *index, char *str, t_token_type *token, int i)
{
	if (i == TK_DQUOTE || i == TK_SQUOTE)
		return (is_quote(index, str, token));
	else if (i == TK_HEREDOC || i == TK_CONCAT)
		*index = *index + 1;
	*token = i;
	return (TRUE);
}

bool is_token(t_token **tk_lst, int *start, int *index, t_token_type *token)
{
	t_token_type prev;
	t_token tokens;
	char *str;
	int i;

	prev = *token;
	i = -1;
	str = sget_input();
	while (++i < 8)
	{
		tokens = sget_tk_spe(i);
		if (!ft_strncmp(tokens.content, &str[*index], tokens.size))
		{
			if (*token == TK_WORD)
			{
				token_add_back(tk_lst, token_new(str, *index - 1, *start, *token));
				*start = *index;
			}
			return (handle_token(index, str, token, i));
		}
	}
	return (FALSE);
}

void tokenisateur(t_token **tk_lst, char *str)
{
	int i;
	int token_start;
	t_token_type token;

	i = 0;
	token_start = 0;
	token = TK_NOTOKEN;
	while (str[i])
	{
		if (is_token(tk_lst, &token_start, &i, &token))
		{
			token_add_back(tk_lst, token_new(str, i, token_start, token));
			token_start = i + 1;
			token = TK_NOTOKEN;
			i++;
		}
		else if (str[i])
		{
			token = TK_WORD;
			i++;
		}
	}
}

// t_cmd **sget_cmd_tab()
// {
// 	static t_cmd **cmd;
// 	size_t i;
// 	size_t nb_cmd;

// 	if (!sget_init(CMD, NOP) && sget_init(CMD, SET))
// 	{
// 		i = -1;
// 		nb_cmd = get_nb_cmd(sget_token()); // traverse les tokens compte le nombre de token_pipe;
// 		cmd = (t_cmd **)malloc(sizeof(t_cmd *) * nb_cmd);
// 		while (++i < nb_cmd)
// 			setup_cmd(cmd[i], sget_token());
// 	}
// 	return (cmd);
// }

t_token **sget_token()
{
	static t_token *token_list = NULL;

	if (!sget_init(TOKEN, NOP) && sget_init(TOKEN, SET))
	{
		printf("imhere\n");
		token_list = NULL;
		tokenisateur(&token_list, sget_input());
	}
	return (&token_list);
}



const char *token_type_to_string(t_token_type type) {
    switch (type) {
        case TK_HEREDOC: return "TK_HEREDOC";
        case TK_CONCAT: return "TK_CONCAT";
        case TK_REDIR_ENT: return "TK_REDIR_ENT";
        case TK_REDIR_EXT: return "TK_REDIR_EXT";
        case TK_SPACE: return "TK_SPACE";
        case TK_PIPE: return "TK_PIPE";
        case TK_SQUOTE: return "TK_SQUOTE";
        case TK_DQUOTE: return "TK_DQUOTE";
        case TK_NOTOKEN: return "TK_NOTOKEN";
        case TK_WORD: return "TK_WORD";
        default: return "Unknown";
    }
}

// Fonction pour afficher une liste chaînée de tokens avec des noms de token en chaîne
void display_token_list(t_token *token_list) {
    t_token *current = token_list;
    while (current) {
        printf("Content: %s, Size: %ld, Token Type: %s\n", current->content, current->size, token_type_to_string(current->token_type));
        current = current->next;
    }
}
