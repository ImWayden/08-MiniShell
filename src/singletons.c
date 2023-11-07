/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singletons.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 03:35:29 by wayden            #+#    #+#             */
/*   Updated: 2023/11/07 18:25:28 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *sget_input()
{
	static char *input;

	if(!sget_init(INPUT, NOP) && sget_init(INPUT, SET))
		input = readline(RED "minishell:>" RESET);
	return (input);
}



t_token *sget_tk_spe(int i)
{
	static const t_token	*tokens[] = {{"<<", 2, TK_HEREDOC},\
	{">>", 2, TK_CONCAT}, {"<", 1, TK_REDIR_ENT}, {">", 1, TK_REDIR_EXT},\
		{" ", 1, TK_SPACE}, {"|", 1, TK_PIPE}, {"\'", 1, TK_SQUOTE},\
		{"\"", 1, TK_DQUOTE}};
	return(tokens[i]);
}