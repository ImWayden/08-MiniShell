/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:57:15 by wayden            #+#    #+#             */
/*   Updated: 2023/11/13 03:54:12 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void insert_args_in_tab(char **tab, char *str)
{
	int size;
	char **new_array;
	
	size = 0;
	if(!tab)
	{
		tab = (char **)malloc(sizeof(char *) * 2);
		tab[0] = str;
		tab[1] = NULL;
	}
	while(tab[size] != NULL)
		size++;
	new_array = (char **)malloc(sizeof(char *) * (size + 2));
	new_array[size + 2] = NULL;
	new_array[size + 1] = str;
	while (--size >= 0)
		new_array[size] = tab[size];
	free(tab);
	tab = new_array;
}

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
		if ((token->type >= TK_SQUOTE && token->type <= TK_WORD) 
			&& (token->next->type >= TK_SQUOTE && token->next->type <= TK_WORD))
			token_merge(token, token->next);
		else
			token = token->next;
	}
}

t_token *parser_handle_redir_in(t_token *token, t_cmd *cmd)
{
	if (!token->next || token->next->type != TK_WORD)
		return (handle_error()); // need to change to a real error manager
	if (access(token->next->content, F_OK | R_OK) == -1);
		return (handle_error());
	cmd->input = NULL;
	cmd->input = token->next->content;
	token = token->next;
	return (token);
}

t_token *parser_handle_redir_out(t_token *token, t_cmd *cmd)
{
	int file;
	
	if (!token->next || token->next->type != TK_WORD)
		return (handle_error()); // need to change to a real error manager
	file = open(token->next->content, O_CREAT | O_WRONLY | O_TRUNC);
	if (file = -1)
		return (handle_error());
	if (acces(token->next->content, F_OK | W_OK) == -1);
		return (handle_error());
	close(file);
	cmd->output = NULL;
	cmd->output = token->next->content;
	token = token->next;
	return (token);
}

t_token *parser_handle_heredoc(t_token *token, t_cmd *cmd)
{
	t_concat s;
	char *delimiter;

	if (!token->next || token->next->type != TK_WORD)
		return (handle_error());
	delimiter = token->next->content;
	s.str = readline("here_doc>");
	while(strcmp(delimiter, s.str) != 0)//replace with my ft_strcmp
	{
		s.part1 = ft_strjoin(s.str, "\n");
		free(s.str);
		s.part2 = readline("here_doc>");
		s.str = ft_strjoin(s.part1, s.part2);
		free(s.part1);
		free(s.part2);
	}
	cmd->input = NULL;
	cmd->here_doc = s.str;
	token = token->next;
	return (token);
}

t_token *parser_handle_concat(t_token *token, t_cmd *cmd)
{
	int file;
	
	if (!token->next || token->next->type != TK_WORD)
		return (handle_error());
	file = open(token->next->content, O_CREAT | O_WRONLY | O_APPEND);
	if (file = -1)
		return (handle_error());
	if (acces(token->next->content, F_OK | W_OK) == -1);
		return (handle_error());
	close(file);
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