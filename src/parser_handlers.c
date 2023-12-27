/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 04:29:33 by wayden            #+#    #+#             */
/*   Updated: 2023/12/28 00:40:14 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*parser_handle_redir_in(t_token *token, t_cmd *cmd)
{
	if (!token->next || token->next->type != TK_WORD)
		handle_error(ERR_MSG_REDIR_IN, NULL, ERR_REDIR_IN);
	p_access(token->next->content, F_OK | R_OK);
	cmd->input = NULL;
	cmd->input = token->next->content;
	token = token->next;
	return (token);
}

t_token	*parser_handle_redir_out(t_token *token, t_cmd *cmd)
{
	int	file;

	if (!token->next || token->next->type != TK_WORD)
		handle_error(ERR_MSG_REDIR_OUT, NULL, ERR_REDIR_OUT);
	file = p_open(token->next->content, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	close(file);
	p_access(token->next->content, F_OK | W_OK);
	cmd->output = NULL;
	cmd->output = token->next->content;
	token = token->next;
	return (token);
}

t_token	*parser_handle_heredoc(t_token *token, t_cmd *cmd)
{
	t_concat	s;
	char		*delimiter;

	if (!token->next || token->next->type != TK_WORD)
		handle_error(ERR_MSG_HEREDOC, NULL, ERR_HEREDOC);
	delimiter = token->next->content;
	s.str = expand(readline("here_doc >"));
	cmd->here_doc = ft_strdup_gc(s.str, 1);
	while (s.str && ft_strcmp(delimiter, s.str) != 0)
	{
		s.part1 = cmd->here_doc;
		cmd->here_doc = ft_strjoin_gc(cmd->here_doc, "\n", 1);
		s.str = expand(readline("here_doc >"));
		if (!s.str || strcmp(delimiter, s.str) == 0)
			break ;
		s.part1 = cmd->here_doc;
		cmd->here_doc = ft_strjoin_gc(cmd->here_doc, s.str, 1);
	}
	if (!s.str)
		ft_putchar_fd('\n', STDOUT_FILENO);
	cmd->input = NULL;
	token = token->next;
	return (token);
}

t_token	*parser_handle_concat(t_token *token, t_cmd *cmd)
{
	int	file;

	if (!token->next || token->next->type != TK_WORD)
		handle_error(ERR_MSG_CONCAT, NULL, ERR_CONCAT);
	file = p_open(token->next->content, O_CREAT | O_WRONLY | O_APPEND, 0666);
	p_access(token->next->content, F_OK | W_OK);
	close(file);
	cmd->output = NULL;
	cmd->concat = token->next->content;
	token = token->next;
	return (token);
}

t_token	*parser_handle_special(t_token *token, t_cmd *cmd)
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
