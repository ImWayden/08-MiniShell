/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:38:11 by wayden            #+#    #+#             */
/*   Updated: 2023/11/05 18:57:36 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>			   // Pour printf
#include <stdlib.h>			   // Pour malloc, free, exit
#include <unistd.h>			   // Pour write, access, close, fork, execve, getpid
#include <readline/readline.h> // Pour readline
#include <readline/history.h>  // Pour rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
#include <signal.h>			   // Pour signal, sigaction, sigemptyset, sigaddset, kill
#include <sys/types.h>		   // Pour wait, waitpid, wait3, wait4, stat, lstat, fstat
#include <sys/stat.h>		   // Pour stat, lstat, fstat
#include <sys/wait.h>		   // Pour wait, waitpid, wait3, wait4
#include <string.h>			   // Pour strerror
#include <errno.h>			   // Pour perror
#include <fcntl.h>			   // Pour open, close
#include <dirent.h>			   // Pour opendir, readdir, closedir
#include <termios.h>		   // Pour tcsetattr, tcgetattr
#include <curses.h>			   // Pour tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
#include <sys/ioctl.h>		   // Pour ioctl
#include <pwd.h>			   // Pour getpwuid
#include <unistd.h>			   // Pour getcwd, chdir
#include <sys/types.h>		   // Pour getcwd, chdir
#include <string.h>			   // Pour getcwd, chdir
#include <stdlib.h>			   // Pour getenv
#include <fcntl.h>			   // Pour dup, dup2, pipe
#include "My_Libft/header/libft.h" //libft_functions


#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"


typedef enum s_refresh
{
	NOP = 0,
	REFRESH = 1	
}	t_refresh;


// typedef struct s_utils
// {
// 	t_env **env;
// 	char **env_ptr;
// 	size_t env_size;
	
// } t_utils;



typedef struct s_env t_env;

struct s_env
{
	char		*name;
	char		*content;
	char		*full;
	t_env		*next;
};

typedef struct s_cmd t_cmd;

struct s_cmd
{
	char	*name;
	char	**args;
	bool	is_builtin;
	bool	has_heredoc;
	t_cmd	*next;
	
};

typedef struct s_minishell_args t_mshellargs;

struct s_minishell_args
{
	char	*name;
	t_mshellargs	*next;
	
};

t_env **sget_env(char **envp);
t_env *env_new(char *str);
void env_add_back(t_env **env, t_env *new);
void env_delone(t_env *var);
char **sget_env_tab(t_refresh refresh);
void env_remove_if(t_env **begin_list, char *name, int (*cmp)());
void refresh_env_tab(void);

void builtin_env(t_env *env);

void clean_env(void);

char *ft_strncpy(char *s1, char *s2, int n);

void builtin_export(char *str);

#endif