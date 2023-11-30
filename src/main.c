/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:36:47 by wayden            #+#    #+#             */
/*   Updated: 2023/11/30 14:28:30 by wayden           ###   ########.fr       */
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
		// printf("%d\n", getpid());
		// fflush(stdout);
		ft_putstr_fd("\n", 2);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
		*sget_exitcode() = 130;
	}
	if (signum == SIGQUIT)
	{
	}
}

int command_handler()
{

	//display_token_list(*sget_token());
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	//printf("%d\n", getpid());
	fflush(stdout);
	if(!*sget_token())
	{
		clean_tokens();
		exit(0);
	}	
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
	exit(*sget_exitcode());
}

void init_vars(char **envp)
{
	sget_init(0, REFRESH);
	sget_env(envp);
	sget_env_tab(NOP);
	sget_scmd(NULL);
	sget_location_flag(ERR_ENV);
}

int main(int argc, char *argv[], char **envp)
{
	pid_t child_pid;
	char *input;
	int status;
	
	signal(SIGINT, inter_signal_handler);
	signal(SIGQUIT, SIG_IGN);
	(void)argc;
	(void)argv;
	init_vars(envp);
	while(1)
	{
		input = sget_input();
		if(!input)
			break;
		child_pid = fork();
		if (!child_pid)
			command_handler();
		signal(SIGINT, SIG_IGN);
		waitpid(child_pid, &status, 0);
		signal(SIGINT, inter_signal_handler);
		*sget_exitcode() = WEXITSTATUS(status);
		if(*sget_exitcode() == RETURN_EXECBACK)
			handle_builtins2(NULL);
		printf("DEBUG : exit code = %d\n", *sget_exitcode());
		p_free((void **)&input);
		clean_scmd();
		sget_init(0, REFRESHALL);
	}
	clean_all();
	rl_clear_history();
	return 0;
}
