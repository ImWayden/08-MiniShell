/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:57:15 by wayden            #+#    #+#             */
/*   Updated: 2023/12/18 19:25:20 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **insert_args_in_tab(char **tab, char *str)
{
	int size;
	char **new_array;

	size = 0;
	// printf("str = %s\n",str);//debug
	if (!tab || !*tab)
	{
		new_array = (char **)p_malloc(sizeof(char *) * 2);
		new_array[0] = str;
		new_array[1] = NULL;
	}
	else
	{
		while (tab[size] != NULL)
			size++;
		new_array = (char **)p_malloc(sizeof(char *) * (size + 2));
		new_array[size + 1] = NULL;
		new_array[size] = str;
		while (--size >= 0)
			new_array[size] = tab[size];
		free(tab);
	}
	return (new_array);
}

void token_merge(t_token *token, t_token *next)
{
	char *str;

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

void merge_quotes(t_token **tokens)
{
	t_token *token;

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

t_token *parser_handle_redir_in(t_token *token, t_cmd *cmd)
{
	if (!token->next || token->next->type != TK_WORD)
		handle_error(ERR_MSG_REDIR_IN, NULL, ERR_REDIR_IN); // need to change to a real error manager
	p_access(token->next->content, F_OK | R_OK);
	cmd->input = NULL;
	cmd->input = token->next->content;
	token = token->next;
	return (token);
}

t_token *parser_handle_redir_out(t_token *token, t_cmd *cmd)
{
	int file;
	if (!token->next || token->next->type != TK_WORD)
		handle_error(ERR_MSG_REDIR_OUT, NULL, ERR_REDIR_OUT); // need to change to a real error manager
	file = p_open(token->next->content, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	close(file);
	p_access(token->next->content, F_OK | W_OK);
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
		handle_error(ERR_MSG_HEREDOC, NULL, ERR_HEREDOC);
	delimiter = token->next->content;
	s.str = expand(readline("here_doc >"));
	cmd->here_doc = ft_strdup(s.str);
	while (s.str && strcmp(delimiter, s.str) != 0) // replace with my ft_strcmp
	{
		s.part1 = cmd->here_doc;
		cmd->here_doc = ft_strjoin(cmd->here_doc, "\n");
		(void)((free(s.part1), 1) && (free(s.str), 1));
		s.str = expand(readline("here_doc >"));
		if (!s.str || strcmp(delimiter, s.str) == 0)
			break ;
		s.part1 = cmd->here_doc;
		cmd->here_doc = ft_strjoin(cmd->here_doc, s.str);
		free(s.part1);
	}
	if (!s.str)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (free(s.str), cmd->input = NULL, token = token->next, token);
}

t_token *parser_handle_concat(t_token *token, t_cmd *cmd)
{
	int file;

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
	static t_token *token;

	token = *tokens;
	if (token && token->type == TK_PIPE)
		token = token->next;
	while (token && token->type != TK_PIPE)
	{
		if (token->type == TK_WORD)
			cmd->args = insert_args_in_tab(cmd->args, token->content);
		else
			token = parser_handle_special(token, cmd);
		token = token->next;
	}
	if (cmd->args)
		cmd->cmd = ft_strdup(cmd->args[0]);
	if (token && token->type == TK_PIPE && !token->next)
		handle_error(ERR_MSG_PIPE, NULL, ERR_PIPE);
	if (token && token->next)
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

void handle_unclosed_pipe(void)
{
	char *input;
	char *s2;
	char *s1;

	input = NULL;
	input = sget_input(NULL);
	s2 = readline(">");
	s1 = ft_strdup(input);
	clean_tokens();
	input = ft_strjoin(s1, s2);
	(void)((free(s1),1) && (free(s2),1));
	sget_init(INPUT,REFRESH);
	sget_input(input);
	printf("input  = %s\n", sget_input(NULL));
	sget_init(TOKEN, REFRESH);
	if (!*sget_token())
	{
		clean_all();
		clean_tokens();
		exit(0);
	}
	verify_voidcommands(sget_token(), get_nb_cmd(sget_token()));
}

void verify_voidcommands(t_token **token_list, int nb_cmd)
{
	t_token *token;
	static int i;
	int is_valid;

	i = 0;
	i++;
	is_valid = 0;
	token = *token_list;
	while (token)
	{
		if (token->type == TK_WORD)
			is_valid = 1;
		if (token->type == TK_PIPE && !is_valid)
			handle_error(ERR_MSG_PIPE, NULL, ERR_PIPE);
		else if (token->type == TK_PIPE && is_valid)
			is_valid = 0;
		token = token->next;
	}
	if (!is_valid && nb_cmd > 1)
		handle_unclosed_pipe();
	if (!is_valid && nb_cmd == 1)
		handle_error(NULL, NULL, ERR_NOCOMMAND);
}

void verify_redir(t_token **token_list)
{
	t_token *token;

	token = *token_list;
	while (token->next)
	{
		if (token->next->type != TK_WORD)
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

void correct_tokenlist(t_token **token_list,int nb_cmd)
{
	// printf("-------------- token list before -----------------\n");
	// display_token_list(*sget_token());
	verify_voidcommands(token_list, nb_cmd);
	clean_quote(token_list);
	expender(token_list);
	merge_quotes(token_list);
	remove_spaces(token_list);
	verify_redir(token_list);
	// printf("-------------- token list after -----------------\n");
	// display_token_list(*sget_token());
}

t_cmd *sget_cmd_tab(void)
{
	static t_cmd *cmd;
	int i;
	t_token **token_list;
	int nb_cmd;

	if (!sget_init(CMD, NOP) && sget_init(CMD, SET))
	{
		i = -1;
		token_list = sget_token();
		if (!*token_list)
			return (NULL);
		nb_cmd = get_nb_cmd(token_list);
		cmd = (t_cmd *)p_malloc(sizeof(t_cmd) * (nb_cmd));
		while (++i < nb_cmd)
		{
			ft_memset(&cmd[i], 0, sizeof(t_cmd) - sizeof(cmd->nb_cmd));
			cmd[i].nb_cmd = nb_cmd;
			token_list = parser(&cmd[i], token_list);
		}
		cmd[0].first = 1;
		cmd[i - 1].last = 1;
	}
	return (cmd);
}