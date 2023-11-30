/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:38:11 by wayden            #+#    #+#             */
/*   Updated: 2023/11/30 05:14:56 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>				   // Pour printf
# include <stdlib.h>				   // Pour malloc, free, exit
# include <unistd.h>				   // Pour write, access, close, fork, execve, getpid
# include <readline/readline.h>	   // Pour readline
# include <readline/history.h>	   // Pour rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
# include <signal.h>				   // Pour signal, sigaction, sigemptyset, sigaddset, kill
# include <sys/types.h>			   // Pour wait, waitpid, wait3, wait4, stat, lstat, fstat
# include <sys/stat.h>			   // Pour stat, lstat, fstat
# include <sys/wait.h>			   // Pour wait, waitpid, wait3, wait4
# include <string.h>				   // Pour strerror
# include <errno.h>				   // Pour perror
# include <fcntl.h>				   // Pour open, close
# include <dirent.h>				   // Pour opendir, readdir, closedir
# include <termios.h>			   // Pour tcsetattr, tcgetattr
# include <curses.h>				   // Pour tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <sys/ioctl.h>			   // Pour ioctl
# include <pwd.h>				   // Pour getpwuid
# include <unistd.h>				   // Pour getcwd, chdir
# include <sys/types.h>			   // Pour getcwd, chdir
# include <string.h>				   // Pour getcwd, chdir
# include <stdlib.h>				   // Pour getenv
# include <fcntl.h>				   // Pour dup, dup2, pipe
# include "My_Libft/header/libft.h" //libft_functions

# define _XOPEN_SOURCE 700
# define RESET "\033[0m"
# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define MAGENTA "\033[1;35m"
# define CYAN "\033[1;36m"
# define WHITE "\033[1;37m"

# define ERR_ENV 0b01
# define ERR_ENV2 0b011
# define ERR_TOKEN 0b0111
# define ERR_PARSER 0b01111
# define ERR_SCMD 0b011111
# define ERR_CLOSE 0x100
# define ERR_OPEN 0x200
# define ERR_MALLOC 0x400
# define ERR_REDIR_IN 0x800
# define ERR_REDIR_OUT 0x1000
# define ERR_HEREDOC 0x2000
# define ERR_CONCAT 0x4000
# define ERR_ACCESS 0x8000
# define ERR_PIPE 0x10000
# define ERR_READL 0x20000
# define ERR_CMD_NOT 0x40000
# define ERR_DUP2 0x80000
# define ERR_CD 0x100000
# define ERR_EXIT 0x200000

# define BUILTINS_EXEC 0x01
# define BUILTINS_NOT_EXEC 0x02
# define BUILTINS_EXEC_BACK 0x04
# define BUILTINS_CD 0x08
# define BUILTINS_PWD 0x10
# define BUILTINS_ECHO 0x20
# define BUILTINS_ENV 0x40
# define BUILTINS_EXIT 0x80
# define BUILTINS_EXPORT 0x100
# define BUILTINS_UNSET 0x200

# define RETURN_EXECBACK 89
# define RETURN_EXIT_NUM_ERR -89

# define ERR_MSG_ENV "env chained list setup"
# define ERR_MSG_ENV2 "env tab setup"
# define ERR_MSG_TOKEN "tokenisateur"
# define ERR_MSG_PARSER "parser"
# define ERR_MSG_SCMD "scmd"
# define ERR_MSG_MALLOC "malloc"
# define ERR_MSG_OPEN	"open : can't open file"
# define ERR_MSG_CLOSE	"close : can't close file"
# define ERR_MSG_REDIR_IN "error next to \'<\'"
# define ERR_MSG_REDIR_OUT "error next to \'>\'"
# define ERR_MSG_HEREDOC "syntax error near unexpected token \'<<\'"
# define ERR_MSG_CONCAT "syntax error near unexpected token \'>>\'"
# define ERR_MSG_ACCESS "permission denied"
# define ERR_MSG_ACCESS_EXIST "file does not exist"
# define ERR_MSG_PIPE "error near \'|\'"
# define ERR_MSG_READL "can\'t read input"
# define ERR_MSG_CMD_NOT "Command not found"
# define ERR_MSG_DUP2 "dup can't dup"
# define ERR_MSG_ARGS "too many arguments"
# define ERR_MSG_CD_HOME "HOME is undefined"
# define ERR_MSG_CD_PWD "OLDPWD is undefined"
# define ERR_MSG_ARG_NUM "numeric argument required"

typedef enum s_refresh
{
	NOP,
	REFRESH,
	SET,
	REFRESHALL
} t_refresh;

// typedef struct s_utils
// {
// 	t_env **env;
// 	char **env_ptr;
// 	size_t env_size;
// } t_utils;
typedef enum e_token_type
{
	TK_HEREDOC,
	TK_CONCAT,
	TK_REDIR_ENT,
	TK_REDIR_EXT,
	TK_SPACE,
	TK_PIPE,
	TK_SQUOTE,
	TK_DQUOTE,
	TK_WORD,
	TK_NOTOKEN
} t_token_type;

typedef int t_error;

typedef enum e_init
{
	TOKEN,
	CMD,
	ENV,
	ENV_TAB,
	INPUT,
	SCMD
} t_init;

typedef struct s_token t_token;

struct s_token
{
	char *content;
	size_t size;
	t_token_type type;
	t_token *next;
};

typedef struct s_env t_env;

struct s_env
{
	char *name;
	char *content;
	char *full;
	bool isok;
	t_env *next;
};

typedef struct s_concat
{
	char *str;
	char *part1;
	char *part2;
	char *part3;
	char *tmp;

} t_concat;


typedef struct s_errorscode
{
	const char	*from;
	const char	*msg;
	t_error		code;
	
}	t_errorcode;


typedef struct s_cmd t_cmd;

struct s_cmd
{
	char *cmd;
	char **args;
	char *input;
	char *output;
	char *here_doc;
	char *concat;
	int is_builtin;
	int	first;
	int last;
	int	nb_cmd; //peut potentiellement etre séparé dans une struct state séparer
};

typedef struct s_scmd
{
	char *cmd;
	char **args;
	int is_builtin;
}	t_scmd;

typedef struct s_minishell_args t_mshellargs;

struct s_minishell_args
{
	char *name;
	t_mshellargs *next;
};

/*
** env related function
*/
t_env **sget_env(char **envp);
t_env *env_new(char *str);
void env_add_back(t_env **env, t_env *new);
void env_delone(t_env *var);
char **sget_env_tab(t_refresh refresh);
void env_remove_if(t_env **begin_list, char *name, int (*cmp)());
void refresh_env_tab(void);
t_env *find_node_by_name(t_env **beign_list, const char *name_to_find);
/*
** token related function
*/
bool sget_init(t_init index, int set);
void display_token_list(t_token *token_list);
void token_delone(t_token *node);
void expender(t_token **tokens);
void clean_quote(t_token **tokens);
void handle_space(t_token **tokens);
/*
** singletons
*/
t_token sget_tk_spe(int i);
char *sget_input();
t_token **sget_token();
t_cmd *sget_cmd_tab(void);
int	*sget_exitcode(void);
t_scmd *sget_scmd(char *name);
/*
** cleaning functions
*/
void clean_env(void);
void clean_tokens(void);
void clean_cmds(void);
void clean_scmd(void);
void clean_all(void);
void cleanhub();
/*
** Utils
*/
int get_nb_cmd(t_token **tokens);
char *ft_strncpy(char *s1, char *s2, int n);
int	ft_simple_atoi_error(const char *nptr);
/*
** Builtins
*/
void builtin_export(t_scmd *cmd);
void builtin_env(t_env *env);
void builtin_exit(char **args);
void builtin_cd(char **args);
void builtin_pwd(void);
void builtin_echo(char **strs, int flag_n);
void builtin_unset(t_scmd *cmd);
/*
**	Error manager
*/
t_error *sget_location_flag(t_error replace);
void handle_error(const char *msg, const char *file ,t_error errorcode);
/*
**	protected functions
*/
char	*p_find_node_by_name(t_env **beign_list, const char *name_to_find);
int p_open(const char *file, int flags, mode_t test);
int p_access(const char *file, int flags);
void *p_malloc(size_t size);
void p_free(void **ptr);
/*
** executor
*/
int main_executor(t_cmd *cmds, char **envp);
void verify_commands(t_cmd *cmds);
void print_cmd(t_cmd *cmd);
char **insert_args_in_tab(char **tab, char *str);
void serialize(t_cmd *cmd, char *name);
void handle_builtins2(char *name);
/*
expand
*/
char *expand(char *content);
#endif