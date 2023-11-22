/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singletons.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 03:35:29 by wayden            #+#    #+#             */
/*   Updated: 2023/11/22 10:58:15 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *sget_input(void)
{
	static char *input = "cmd arg1 < redir_in > redir_out | test";
	// printf("%d",(!sget_init(INPUT, NOP)));
	// printf("%d", sget_init(INPUT, SET));
	if(!sget_init(INPUT, NOP) && sget_init(INPUT, SET))
	{
		input = readline(RED "minishell:>" RESET);
		add_history(input);
	}
	return (input);
}

bool sget_init(t_init index, int set)
{
	static bool init_tab[6] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
	int			i;

	i = 0;
	if (set == REFRESH)
		init_tab[index] = FALSE;
	else if (set == SET)
		init_tab[index] = TRUE;
	else if (set == REFRESHALL)
		ft_memset(&init_tab,FALSE,sizeof(bool) * 6);
	return (init_tab[index]);
}

t_token sget_tk_spe(int i)
{
	static const t_token	tokens[8] = {{"<<", 2, TK_HEREDOC},\
	{">>", 2, TK_CONCAT}, {"<", 1, TK_REDIR_ENT}, {">", 1, TK_REDIR_EXT},\
		{" ", 1, TK_SPACE}, {"|", 1, TK_PIPE}, {"\'", 1, TK_SQUOTE},\
		{"\"", 1, TK_DQUOTE}};
	return(tokens[i]);
}


int	*sget_exitcode(void)
{
	static int exitcode = 0;
	return(&exitcode);
}