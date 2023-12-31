/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   void_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 06:12:17 by wayden            #+#    #+#             */
/*   Updated: 2024/01/01 22:22:25 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_things(t_cmd *cmd)
{
	if (!cmd)
		return ;
	close(cmd->out);
	close(cmd->in);
	close(STDIN_FILENO);
}

static int	validity_checker(int is_valid, t_token *token)
{
	if (!is_valid)
		is_valid = 1;
	if ((token->content && token->content[0]) || token->was_quote)
		is_valid = 3;
	return (is_valid);
}

void	verify_voidcommands(t_token **token_list, int nb_cmd)
{
	t_token		*token;
	static int	i;
	int			is_valid;

	i = 0;
	i++;
	is_valid = 0;
	token = *token_list;
	while (token)
	{
		if (token->type == TK_WORD)
			is_valid = validity_checker(is_valid, token);
		if (token->type == TK_PIPE && !is_valid)
			handle_error(ERR_MSG_PIPE, NULL, ERR_PIPE);
		else if (token->type == TK_PIPE && is_valid)
			is_valid = 0;
		token = token->next;
	}
	if (!is_valid && nb_cmd > 1)
		handle_unclosed_pipe();
	if (is_valid < 3 && nb_cmd == 1)
		handle_error(NULL, NULL, ERR_NOCOMMAND);
}
