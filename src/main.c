/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:36:47 by wayden            #+#    #+#             */
/*   Updated: 2023/12/29 05:13:59 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int	command_handler(void)
{
	signal(SIGINT, child_signal_handler);
	signal(SIGQUIT, child_signal_handler);
	if (!*sget_token())
	{
		clean_tokens();
		free_all_garbage();
		exit(0);
	}
	correct_tokenlist(sget_token(), get_nb_cmd(sget_token()));
	sget_location_flag(ERR_TOKEN);
	sget_cmd_tab();
	sget_location_flag(ERR_PARSER);
	verify_commands(sget_cmd_tab());
	main_executor(sget_cmd_tab(), sget_env_tab(NOP));
	clean_all();
	clean_tokens();
	clean_cmds();
	free_all_garbage();
	exit(*sget_exitcode());
}

void	init_vars(char **envp)
{
	sget_init(0, REFRESH);
	sget_env(envp);
	sget_env_tab(NOP);
	sget_scmd(NULL);
	sget_location_flag(ERR_ENV);
}

void	verify_activedirectory(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	add_tmp_garbage(cwd);
	if (cwd == NULL && errno == ENOENT)
		chdir(sget_abspath());
}

void	main_loop(void)
{
	pid_t	child_pid;
	int		status;

	child_pid = fork();
	if (!child_pid)
		command_handler();
	waitpid(child_pid, &status, 0);
	*sget_exitcode() = WEXITSTATUS(status);
	if (*sget_exitcode() == RETURN_EXECBACK)
		handle_builtins2(NULL, NULL);
	clean_scmd();
	sget_init(0, REFRESHALL);
	free_tmp_garbage();
}

int	main(int argc, char *argv[], char **envp)
{
	char	*input;

	signal(SIGINT, input_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	(void)argc;
	(void)argv;
	init_vars(envp);
	sget_abspath();
	while (1)
	{
		verify_activedirectory();
		input = sget_input(NULL);
		if (!input)
			break ;
		main_loop();
	}
	clean_all();
	rl_clear_history();
	free_all_garbage();
	return (0);
}
