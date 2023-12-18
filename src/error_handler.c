/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 03:56:12 by wayden            #+#    #+#             */
/*   Updated: 2023/12/18 19:09:08 by wayden           ###   ########.fr       */
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
	if(flag & ERR_ENV)
		msg = ERR_MSG_ENV;
	if(flag & ERR_TOKEN)
		msg = ERR_MSG_TOKEN;
	if(flag & ERR_PARSER)
		msg = ERR_MSG_PARSER;
	if(flag & ERR_SCMD)
		msg = ERR_MSG_SCMD;
	return(msg);
}

void cleanhub()
{
	t_error		flag;
	
	flag = *sget_location_flag(0);
	if(flag & ERR_ENV)
		clean_env();
	if(flag & ERR_TOKEN)
		clean_tokens();
	if(flag & ERR_PARSER)
		clean_cmds();
	if(flag & ERR_SCMD)
		clean_scmd();
}

t_error change_exitcode(t_error errcode)
{
	if (errcode == ERR_NOCOMMAND)
		return(0);
	else if(errcode == ERR_ACCESS)
		return(1);
	else if(errcode == ERR_CD)
		return(1);
	else if (errcode == ERR_EXIT || errcode == ERR_PIPE)
		return (2);
	else
		return(127);
	return(0);
}

void handle_error(const char *msg, const char *file ,t_error errorcode)
{
	char	*from;
	t_error				exitcode;
	
	from = manage_location();
	exitcode = change_exitcode(errorcode);//a remplacer par la fonction qui choisit quel exit code entrer
	if(*sget_exitcode() == RETURN_EXECBACK)
	{
		ft_putstr_fd((char *)msg,STDERR_FILENO);
		ft_putchar_fd('\n',STDERR_FILENO);
		*sget_exitcode() = 1;
		return;
	}
	if(errorcode & ERR_CLOSE || errorcode & ERR_OPEN || errorcode & ERR_ACCESS)
		printf("minishell : %s : %s : %s\n", from, file, msg);
	else if(!(errorcode & ERR_NOCOMMAND))
		printf("minishell : %s : %s\n", from, msg);
	cleanhub();
	exit(exitcode);
}