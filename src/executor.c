/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 01:37:14 by wayden            #+#    #+#             */
/*   Updated: 2023/11/21 03:43:57 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_outs(int *in, int *out, t_cmd *cmd, int pipe_fd[2])
{
	if (cmd->output)
	{
		out = p_open(cmd->output, O_CREAT | O_WRONLY | O_TRUNC, 0666);
		p_close(pipe_fd[1]);
	}
	else if (cmd->concat)
	{
		out = p_open(cmd->concat, O_CREAT | O_WRONLY | O_APPEND, 0666);
		p_close(pipe_fd[1]);
	}
	else
		out = pipe_fd[1];
	p_dup2(out, STDOUT_FILENO);
	p_close(out);
	p_close(pipe_fd[0]);
}

static void	setup_ins(int *in, int *out, t_cmd *cmd, int pipe_fd[2])
{
	int here_doc[2];

	if (cmd->input)
	{
		in = p_open(cmd->input, O_RDONLY, 0666);
		p_close(pipe_fd[0]);
	}
	else if(cmd->here_doc)
	{
		pipe(here_doc);
		p_write(here_doc[1], &cmd->here_doc, ft_strlen(cmd->here_doc));
		in = here_doc[0];
		p_close(here_doc[1]);
		p_close(pipe_fd[0]);
	}
	else
		in = pipe_fd[0];
	p_dup2(in, STDIN_FILENO);
	p_close(in);
	p_close(pipe_fd[1]);
}

static void exec_process(t_cmd *cmd, char **env, int pipe_fd[2])
{
	int in;
	int out;
	
	setup_ins(&in, &out, cmd, pipe_fd);
	setup_outs(&in, &out, cmd, pipe_fd);
	if (is_builtin(cmd->cmd))
		exit(2);//change with error code to indicate a builtin
	execve(cmd->cmd, cmd->args, env);
}



void launch_process(t_cmd *cmd, int pipe_fd[2], char **env)
{
	exec_process(cmd,env,pipe_fd);
}

int executor(int pipe_fd[2], long int i_argc[2], t_cmd *cmd, char **envp)
{
	int pid;
	int *i;

	i = (int *)i_argc[0];
	while (*i < i_argc[1])
	{
		pipe(pipe_fd);
		pid = fork();
		if (!pid)
			launch_process(&cmd[*i], pipe_fd, envp);
		p_dup2(pipe_fd[0], STDIN_FILENO);
		p_close(pipe_fd[0]);
		p_close(pipe_fd[1]);
		*i += 1;
	}
	p_close(0);
	return (0);
}

int main_executor(t_cmd *cmds, char **envp)
{
	int i;
	int nb_cmd;
	int pipe_fd[2];

	get_command(NULL, get_env_name(get_env_shell(envp)));
	nb_cmd = cmds->nb_cmd;
	i = 0;
	if (executor(pipe_fd, (long int[]){(long int)&i, nb_cmd}, cmds, envp) == 2)
		exit(2);
	while (i--)
		wait(NULL);
	return (0);
}
