/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 01:37:14 by wayden            #+#    #+#             */
/*   Updated: 2023/12/31 01:04:02 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_ins(int in, t_cmd *cmd, int pipe_fd[2])
{
	int	here_doc[2];

	if (cmd->input)
		in = p_open(cmd->input, O_RDONLY, 0666);
	else if (cmd->here_doc)
	{
		pipe(here_doc);
		write(here_doc[1], cmd->here_doc, ft_strlen(cmd->here_doc));
		p_dup2(here_doc[0], STDIN_FILENO, "in");
		p_close(here_doc[1], "pipe here_doc");
		p_close(here_doc[0], "pipe here_doc");
	}
	if (in != STDIN_FILENO && (cmd->first == 1
			|| (cmd->first == 0 && in != pipe_fd[0])))
		p_dup2(in, STDIN_FILENO, "in");
	p_close(pipe_fd[0], "closed reading pipe on first process");
	return (in);
}

static int	setup_outs(int out, t_cmd *cmd, int pipe_fd[2])
{
	int	f;

	f = 0;
	if (cmd->output)
	{
		out = p_open(cmd->output, O_CREAT | O_WRONLY | O_TRUNC, 0666);
		p_close(pipe_fd[1], "pipe 1");
		f++;
	}
	else if (cmd->concat)
	{
		out = p_open(cmd->concat, O_CREAT | O_WRONLY | O_APPEND, 0666);
		p_close(pipe_fd[1], "pipe 2");
		f++;
	}
	else if (cmd->last == 0)
		out = pipe_fd[1];
	if (out != STDOUT_FILENO)
		p_dup2(out, STDOUT_FILENO, "outs");
	if (!f)
		p_close(pipe_fd[1], "pipe 3");
	return (out);
}

void	close_things(t_cmd *cmd)
{
	if (!cmd)
		return ;
	close(cmd->out);
	close(cmd->in);
	close(STDIN_FILENO);
}

static void	launch_process(t_cmd *cmd, int pipe_fd[2], char **env, int n)
{
	int	in;
	int	out;

	in = STDIN_FILENO;
	out = STDOUT_FILENO;
	cmd->in = setup_ins(in, cmd, pipe_fd);
	cmd->out = setup_outs(out, cmd, pipe_fd);
	sget_pipe_g_exit(PIPE_WRITE | PIPE_CLOSE);
	if (cmd->type == TYPE_BUILTIN)
		builtin_handler(cmd, n);
	if (cmd->type == TYPE_NOTCMD)
		handle_error(ERR_MSG_CMD_NOT, cmd->cmd, ERR_CMD_NOT);
	if (cmd->type == TYPE_DIR)
		handle_error(ERR_MSG_ISDIR, cmd->cmd, ERR_ISDIR);
	if (cmd->type == TYPE_NOCMD)
		handle_error(NULL, NULL, ERR_NOCOMMAND);
	if (cmd->type == TYPE_EXEC)
		execve(cmd->cmd, cmd->args, env);
	close_things(cmd);
	free_all_garbage();
	rl_clear_history();
	free_all_garbage();
	exit(127);
}

int	executor(int pipe_fd[2], long int i_argc[2], t_cmd *cmd, char **envp)
{
	int	pid;
	int	*i;
	int	status;

	i = (int *)i_argc[0];
	while (*i < i_argc[1])
	{
		pipe(pipe_fd);
		pid = fork();
		if (!pid)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			launch_process(&cmd[*i], pipe_fd, envp, *i);
		}
		p_dup2(pipe_fd[0], STDIN_FILENO, "main");
		p_close(pipe_fd[0], "pipe");
		p_close(pipe_fd[1], "pipe");
		*i += 1;
	}
	p_close(0, "stdin");
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	main_executor(t_cmd *cmds, char **envp)
{
	int	i;
	int	nb_cmd;
	int	pipe_fd[2];

	nb_cmd = cmds->nb_cmd;
	i = 0;
	*sget_exitcode() = \
	executor(pipe_fd, (long int []){(long int)&i, nb_cmd}, cmds, envp);
	sget_pipe_g_exit(PIPE_WRITE | PIPE_CLOSE);
	while (i--)
		wait(NULL);
	rl_clear_history();
	return (*sget_exitcode());
}
