/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 01:37:14 by wayden            #+#    #+#             */
/*   Updated: 2023/11/22 03:48:42 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void p_close(int fd, char *file)
{
	int cl;

	cl = close(fd);
	if(cl == -1)
		handle_error(ERR_MSG_CLOSE, file, ERR_CLOSE);
}

void p_dup2(int fd, int fd2, char *debug)
{
	int dp;
	
	dp = dup2(fd, fd2);
	if(dp == -1)
	{
		printf("%s\n", debug);
		handle_error(ERR_MSG_DUP2, NULL, ERR_DUP2);
	}

}

bool is_builtin(char *cmd)
{
	if(!strcmp(cmd, "cd") || !strcmp(cmd, "exit") ||!strcmp(cmd, "export") 
		|| !strcmp(cmd, "pwd") || !strcmp(cmd, "unset") || !strcmp(cmd, "echo") 
			|| !strcmp(cmd,"env"))
		return (TRUE);
	else
		return (FALSE);
}

static void	setup_ins(int in,t_cmd *cmd, int pipe_fd[2])
{
	int here_doc[2];
	t_cmd *cmd2;
	
	cmd2 = &cmd[1];
	if (cmd->input)
		in = p_open(cmd->input, O_RDONLY, 0666);
	else if(cmd->here_doc)
	{
		pipe(here_doc);
		write(here_doc[1], cmd->here_doc, ft_strlen(cmd->here_doc));
		p_dup2(here_doc[0], STDIN_FILENO, "in");
		p_close(here_doc[1], "pipe here_doc");
	}
	if(in != STDIN_FILENO && (cmd->first == 1 
		|| (cmd->first == 0 && in != pipe_fd[0])))
		p_dup2(in, STDIN_FILENO, "in");
	if(cmd->first == 1 || cmd->last == 1)
		p_close(pipe_fd[0], "closed reading pipe on first process");
}

static void	setup_outs(int out, t_cmd *cmd, int pipe_fd[2])
{
	if (cmd->output)
	{
		out = p_open(cmd->output, O_CREAT | O_WRONLY | O_TRUNC, 0666);
		p_close(pipe_fd[1], "pipe 1");
	}
	else if (cmd->concat)
	{
		out = p_open(cmd->concat, O_CREAT | O_WRONLY | O_APPEND, 0666);
		p_close(pipe_fd[1], "pipe 2");
	}
	else if(cmd->last == 0)
		out = pipe_fd[1];
	if(out != STDOUT_FILENO)
		p_dup2(out, STDOUT_FILENO, "outs");
	p_close(pipe_fd[1], "pipe 3");
}	

static void exec_process(t_cmd *cmd, char **env, int pipe_fd[2])
{
	int in;
	int out;
	
	in = STDIN_FILENO;
	out = STDOUT_FILENO;
	setup_ins(in, cmd, pipe_fd); //problème de redirection en cas de premier et dernier executable a executer
	setup_outs(out, cmd, pipe_fd);
	if (cmd->is_builtin == 2)// shoudl have been set in command verifier
		exit(2);//change with error code to indicate a non exec builtin + need to close the pipe_fd perhaps ?
	else if(cmd->is_builtin == 1)
		exit(2);//change with error code to indicate a executable builtin + need to close the pipe_fd perhaps so probably to make a proper exit function ?
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
		//printf("process = %d\n  pipe_fd[0] = %d\n  pipe_fd[1] = %d\n",*i, pipe_fd[0], pipe_fd[1]); //debug
		if (!pid)
			launch_process(&cmd[*i], pipe_fd, envp);
		p_dup2(pipe_fd[0], STDIN_FILENO, "main");
		p_close(pipe_fd[0], "pipe");
		p_close(pipe_fd[1], "pipe");
		*i += 1;
	}
	p_close(0, "stdin");
	return (0);
}

int main_executor(t_cmd *cmds, char **envp)
{
	int i;
	int nb_cmd;
	int pipe_fd[2];

	nb_cmd = cmds->nb_cmd;
	i = 0;
	if (executor(pipe_fd, (long int[]){(long int)&i, nb_cmd}, cmds, envp) == 2)
		exit(2);
	while (i--)
		wait(NULL);
	return (0);
}


