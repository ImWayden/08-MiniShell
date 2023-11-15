/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 03:56:12 by wayden            #+#    #+#             */
/*   Updated: 2023/11/15 03:00:09 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



t_error *sget_location_flag(t_error replace)
{
	static t_error flag = 0;

	if(replace)
		flag = replace;
	return (&flag);
}

char *manage_location()
{
	t_error		flag;
	char	*msg;
	
	flag = *sget_location_flag(0);
	if(flag & ERR_PARSER)
		(void)((clean_cmds(), 1) && (msg = ERR_MSG_PARSER, 1));
	if(flag & ERR_TOKEN)
		(void)((clean_tokens(), 1) && (msg = ERR_MSG_TOKEN, 1));
	// if(flag & ERR_ENV2)
	// 	(void)((free(sget_env_tab(NOP)), 1) && (msg = ERR_MSG_ENV2, 1));
	if(flag & ERR_ENV)
		(void)((clean_env(), 1) && (msg = ERR_MSG_ENV, 1));
	return(msg);
}

void handle_error(const char *msg, const char *file ,t_error errorcode)
{
	char	*from;
	t_error				exitcode;
	
	from = manage_location();
	exitcode = 0;//a remplacer par la fonction qui choisit quel exit code entrer
	if(errorcode & ERR_CLOSE || errorcode & ERR_OPEN)
		printf("minishell : %s : %s : %s", from, file, msg);
	else
		printf("minishell : %s : %s", from, msg);
	exit(exitcode);
}