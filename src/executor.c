/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 01:37:14 by wayden            #+#    #+#             */
/*   Updated: 2023/11/22 11:56:42 by wayden           ###   ########.fr       */
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
void builtin_executor(t_cmd *cmd)
{
	if(cmd->is_builtin & BUILTINS_PWD)
		builtin_pwd();
	else if(cmd->is_builtin & BUILTINS_ENV)
		builtin_env(*sget_env(NULL));
	else if(cmd->is_builtin & BUILTINS_ECHO)
		builtin_echo(cmd->args, 0); //remplacer par cmd probablement comme ca on ne se retrouve pas emmerder
	cleanhub();
	exit(0);
}

void handle_builtins2(void)
{
	t_scmd *cmd;
	
	cmd = sget_scmd();
	if(cmd->is_builtin & BUILTINS_CD)
		builtin_cd(cmd->cmd);
	else if(cmd->is_builtin & BUILTINS_EXPORT)
		builtin_export(cmd->cmd);//replace with t_scmd
	else if(cmd->is_builtin & BUILTINS_UNSET)
		builtin_unset(cmd->cmd);//replace with t_scmd
	else if(cmd->is_builtin & BUILTINS_EXIT)
		builtin_exit();
}

void builtin_handler(t_cmd *cmd)
{
	if(cmd->is_builtin & BUILTINS_EXEC)
		builtin_executor(cmd);
	else if(cmd->is_builtin & BUILTINS_NOT_EXEC)
	{
		cleanhub();
		exit(0);//idk the code but 0 zero seems ok
	}
	else if(cmd->is_builtin & BUILTINS_EXEC_BACK)
	{
		serialize(cmd);
		cleanhub();
		exit(RETURN_EXECBACK);
	}
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
	if (cmd->is_builtin)// shoudl have been set in command verifier
		builtin_handler(cmd);//change with error code to indicate a non exec builtin + need to close the pipe_fd perhaps ?
	else
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
	int status;
	
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
	if(i == 1)
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));		
	}
	return (0);
}

int main_executor(t_cmd *cmds, char **envp)
{
	int i;
	int nb_cmd;
	int pipe_fd[2];

	nb_cmd = cmds->nb_cmd;
	i = 0;
	if (executor(pipe_fd, (long int[]){(long int)&i, nb_cmd}, cmds, envp) == RETURN_EXECBACK)
		exit(RETURN_EXECBACK);
	if(i > 1)
		while (i--)
			wait(NULL);
	return (0);
}
