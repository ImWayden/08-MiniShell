/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singletons.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 03:35:29 by wayden            #+#    #+#             */
/*   Updated: 2024/01/01 22:07:16 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*sget_input(char *str)
{
	static char	*input;

	if (!sget_init(INPUT, NOP) && sget_init(INPUT, SET))
	{
		if (str)
		{
			if (input && *input)
				free(input);
			input = str;
		}
		else
		{
			input = readline("minishell$ ");
			add_history(input);
		}
	}
	return (input);
}

bool	sget_init(t_init index, int set)
{
	static bool	init_tab[7] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

	if (set == REFRESH)
		init_tab[index] = FALSE;
	else if (set == SET)
		init_tab[index] = TRUE;
	else if (set == REFRESHALL)
		ft_memset(&init_tab, FALSE, sizeof(bool) * 7);
	return (init_tab[index]);
}

t_token	sget_tk_spe(int i)
{
	static const t_token	tokens[8] = {{"<<", 2, TK_HEREDOC, NULL}, \
	{">>", 2, TK_CONCAT, NULL}, {"<", 1, TK_REDIR_ENT, NULL}, \
	{">", 1, TK_REDIR_EXT, NULL}, {" ", 1, TK_SPACE, NULL}, \
	{"|", 1, TK_PIPE, NULL}, {"\'", 1, TK_SQUOTE, NULL}, \
	{"\"", 1, TK_DQUOTE, NULL}};

	return (tokens[i]);
}

int	*sget_exitcode(void)
{
	static int	exitcode = 0;

	return (&exitcode);
}

char	*sget_abspath(void)
{
	static char	*path;
	static int	is_init = 0;

	if (!is_init)
	{
		path = getcwd(NULL, 0);
		add_garbage(path, 0);
		is_init = 1;
	}
	return (path);
}
