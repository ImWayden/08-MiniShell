/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:57:15 by wayden            #+#    #+#             */
/*   Updated: 2023/11/12 02:01:31 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void token_merge(t_token *token, t_token *next)
{
	char *str;
	size_t size;

	if (token->type == TK_SPACE)
	{
		str = ft_strdup("");
		token->size = 0;
	}
	else
	{
		str = ft_strjoin(token->content, next->content);
		token->size += next->size;
	}
	token->next = next->next;
	token->type = TK_WORD;
	free(token->content);
	token_delone(next);
	token->content = str;
}

void remove_spaces(t_token **tokens)
{
	t_token *token;
	t_token *tmp;

	token = *tokens;
	while (token->next)
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

void merge_quotes(t_token **tokens)
{
	t_token *token;

	token = *tokens;
	while (token->next)
	{
		if ((token->type >= TK_SQUOTE && token->type <= TK_WORD) && (token->next->type >= TK_SQUOTE && token->next->type <= TK_WORD))
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

// expand exitcode todo
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
	int k;
	int i;

	i = 0;
	while (token->content[i])
	{
		if (token->content[i] == '$' && token->content[i + 1])
		{
			if (ft_isalnum(token->content[i + 1]))
				expand_var(&i, token);
			// else if (str[i + 1] == '?')
			// 	expand_exitcode(&i, token);
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

t_token *parser_handle_redir_in(t_token *token, t_cmd *cmd)
{
	if (!token->next || token->next->type != TK_WORD)
	return (handle_error()); // need to change to a real error manager
	try_open(token->next->content);
	cmd->input = NULL;
	cmd->input = token->next->content;
	token = token->next;
	return (token)
}

t_token *parser_handle_redir_out(t_token *token, t_cmd *cmd)
{
	if (!token->next || token->next->type != TK_WORD)
		return (handle_error()); // need to change to a real error manager
	try_create(token->next->content);
	cmd->output = NULL;
	cmd->output = token->next->content;
	token = token->next;
	return (token)
}

t_token *parser_handle_heredoc(t_token *token, t_cmd *cmd)
{
	if (!token->next || token->next->type != TK_WORD)
		return (handle_error());
	cmd->input = NULL;
	cmd->here_doc = handle_heredoc(token);
	token = token->next;
	return (token);
}

t_token *parser_handle_concat(t_token *token, t_cmd *cmd)
{
	if (!token->next || token->next->type != TK_WORD)
		return (handle_error());
	try_create_concat(token->next->content);
	cmd->output = NULL;
	cmd->concat = token->content;
	token = token->next;
	return (token);
}

t_token *parser_handle_special(t_token *token, t_cmd *cmd)
{
	if (token->type == TK_REDIR_ENT)
		token = parser_handle_redir_in(token, cmd);
	else if (token->type == TK_REDIR_EXT)
		token = parser_handle_redir_out(token, cmd);
	else if (token->type == TK_HEREDOC)
		token = parser_handle_heredoc(token, cmd);
	else if (token->type == TK_CONCAT)
		token = parser_handle_concat(token, cmd);

	return (token);
}

t_token **parser(t_cmd *cmd, t_token **tokens)
{
	bool has_cmd_been_found;
	t_token *token;

	token = *tokens;
	has_cmd_been_found = FALSE;
	if (token->type == TK_PIPE)
		token = token->next;
	while (token && token->type != TK_PIPE)
	{
		if (token->type == TK_WORD && !has_cmd_been_found)
		{
			has_cmd_been_found = TRUE;
			cmd->cmd = token->content;
		}
		else if (token->type == TK_WORD)
			insert_args_in_tab(cmd->args, token->content);
		else
			parser_handle_special(token, cmd);
		token = token->next;
	}
	if (token && token->type == TK_PIPE && !token->next)
		return (handle_error());
	token = token->next;
	return (&token);
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
		if (start->type == TK_PIPE)
			nb_cmd++;
		start = start->next;
	}
	return (nb_cmd);
}

void correct_tokenlist(t_token **token_list)
{
	clean_quote(token_list);
	expender(token_list);
	merge_quotes(token_list);
	remove_spaces(token_list);
}

t_cmd *sget_cmd_tab(void)
{
	static t_cmd *cmd;
	size_t i;
	t_token **token_list;
	int nb_cmd;

	token_list = sget_token();
	if (!sget_init(CMD, NOP) && sget_init(CMD, SET))
	{
		i = -1;
		nb_cmd = get_nb_cmd(token_list);
		cmd = (t_cmd *)malloc(sizeof(t_cmd) * nb_cmd);
		correct_tokenlist(token_list);
		while (++i < nb_cmd)
			token_list = parser(&cmd[i], token_list);
	}
	return (cmd);
}