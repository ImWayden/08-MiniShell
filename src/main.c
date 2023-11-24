/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:36:47 by wayden            #+#    #+#             */
/*   Updated: 2023/11/24 03:37:19 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

/*
**
** testing parts
** 
*/

void print_cmd(t_cmd *cmd) {
    if (cmd == NULL) {
        printf("Structure de commande NULL\n");
        return;
    }
	t_cmd current;
	current = cmd[0];
	int i = 0;
	int nb_cmd = cmd[0].nb_cmd;

		printf("Commande: %s\n", current.cmd);
		printf("Arguments:\n");
		char **arg = current.args;
		while (arg && *arg != NULL) {
			printf("  %s\n", *arg);
			arg++;
		}
		printf("Entrée: %s\n", current.input);
		printf("Sortie: %s\n", current.output);
		printf("Here_doc: %s\n", current.here_doc);
		printf("Concat: %s\n", current.concat);
		printf("Builtin: %d\n", current.is_builtin);
		i++;
		if(i < nb_cmd)
			current = cmd[i];
		// Vous pouvez également appeler récursivement print_cmd pour afficher la structure suivante
}



void inter_signal_handler(int signum) 
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 2);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int command_handler()
{

	//display_token_list(*sget_token());
	sget_location_flag(ERR_TOKEN);
	sget_cmd_tab();
	//display_token_list(*sget_token());
	//print_cmd(sget_cmd_tab());//debug
	sget_location_flag(ERR_PARSER);
	verify_commands(sget_cmd_tab());
	main_executor(sget_cmd_tab(), sget_env_tab(NOP));
	clean_env();
	clean_tokens();
	clean_cmds();
	exit(EXIT_SUCCESS);
}

void init_vars(char **envp)
{
	sget_init(0, REFRESH);
	sget_env(envp);
	sget_env_tab(NOP);
	sget_location_flag(ERR_ENV);
}



int main(int argc, char *argv[], char **envp)
{
	pid_t child_pid;
	char *input;
	int exit_code;
	int status;
	struct sigaction sa;
	
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &inter_signal_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	int i;

	i = 0;
	init_vars(envp);
	while(1)
	{
		input = sget_input();
		if(!input)
			break;
		child_pid = fork();
		if (!child_pid)
			command_handler();
		waitpid(child_pid, &status, 0);
		exit_code = WEXITSTATUS(status);
		if(exit_code == RETURN_EXECBACK)
			handle_builtins2();
		else
			*sget_exitcode() = exit_code; 
		//printf("DEBUG : exit code = %d\n", exit_code);
		p_free((void **)&input);
		sget_init(0, REFRESHALL);
	}
	clean_all();
	//rl_clear_history();
	return 0;
}
