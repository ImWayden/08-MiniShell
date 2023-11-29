/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:15:36 by wayden            #+#    #+#             */
/*   Updated: 2023/11/29 03:22:40 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	fonctionne bien a priori.
**	a priori pas besoin de quitter le programme en cour
**
*/
void builtin_env(t_env *env)
{
	while (env != NULL)
	{
		if(env->isok == TRUE)
		{
			ft_putstr_fd(env->name, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(env->content, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		env = env->next;
	}
	exit(0);
}

void print_export_env(char c)
{
	t_env *env;

	env = *sget_env(NULL);
	while (env != NULL)
	{
		if(env->name[0] == c)
			ft_putstr_fd("declare -x ",STDOUT_FILENO);
		if(env->isok == TRUE && env->name[0] == c)
		{
			ft_putstr_fd(env->name, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putchar_fd('\"', STDOUT_FILENO);
			ft_putstr_fd(env->content, STDOUT_FILENO);
			ft_putchar_fd('\"', STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		if(env->isok == FALSE && env->name[0] == c)
		{
			ft_putstr_fd(env->name, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		env = env->next;
	}
}
/*
**
** 	fonctionne a priori peut etre beosin de plus de tests
**  ne fonctionne pas dans les commande piped
**
*/
void export_utils2(char *args, int j, t_env *env)
{
	char *content;
	
	content = ft_strdup(&args[j + 1]);
	free(env->content);
	env->content = content;
	free(env->full);
	env->full = ft_strdup(args);
}

void export_utils1(char *args, int j, t_env *env)
{
	char *content;

	while (args[j] && args[j] != '=')
		j++;	
	content = ft_substr(args, 0, j);
	env = find_node_by_name(sget_env(NULL), content);
	free(content);
	if (env && args[j])
		export_utils2(args, j, env);
	else if(ft_isalpha(args[0]))
		env_add_back(sget_env(NULL), env_new(args));
	else
	{
		ft_putstr_fd("bash: export: not a valid identifier\n", STDERR_FILENO);
		*sget_exitcode() = 1;
	}
}

void builtin_export(t_scmd *cmd)
{
	int i;
	t_env *env;
	char	c;

	i = -1;
	while (cmd->args && cmd->args[++i])
		export_utils1(cmd->args[i], 0, env);
	if (!cmd->args)
	{
		c = 'A';
		while(c <= 'Z')
		{
			print_export_env(c);
			c++;
		}
		c = 'a';
		while(c <= 'z')
		{
			print_export_env(c);
			c++;
		}
	}
	refresh_env_tab();
}

/*
**	should work flawlessly
**	just besoin de changer strcmp en ft_strcmp
**	que je n'ai malheureusement pas dans ma libft
*/
void builtin_unset(t_scmd *cmd)
{
	int i;

	i = -1;
	while (cmd->args && cmd->args[++i])
		env_remove_if(sget_env(NULL), cmd->args[i], strcmp); //need some debugging idk why it does not work with the new export peprhaps because of \n char ? idk
	refresh_env_tab();
}

/*
** executable si la commande contient des pipes
**
**
*/
void builtin_echo(char **args, int flag_n)
{
	int i;
	int j;
	
	flag_n = 0;
	i = 1;
	j = 2;
	if (args[1][0] == '-' && args[1][1] == 'n')
	{
		while(args[i][j] == 'n')
			j++;
		if(!args[i][j])
		{
			flag_n = 1;
			i++;
		}
	}
	while (args && args[i] != NULL)
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		i++;
		if (args[i] != NULL)
			write(STDOUT_FILENO, " ", 1);
	}
	if (!flag_n)
		write(STDOUT_FILENO, "\n", 1);
}

/*
** should work verify if no args or if args is unique and if it's numerical
** then clean what's malloc and exit with custom or precedent exit code
**
*/
void builtin_exit(char **args, int is_builtin)
{
	int *exit_code;

	exit_code = sget_exitcode();
	if(args && args[0] && args[1])
		handle_error(ERR_MSG_ARGS, "exit", ERR_EXIT);
	if (args && args[0])
	{
		*exit_code = ft_simple_atoi_error(args[0]);
		if(*exit_code == RETURN_EXIT_NUM_ERR)
			handle_error(ERR_MSG_ARG_NUM, "exit", ERR_EXIT);
	}
	clean_all();
	exit(*exit_code);
}
/*
** builtin pwd executable dans ligne
** de commadne contenant des pipes
**
*/
void builtin_pwd(void)
{
	char *cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	printf("%s\n", cwd);
	free(cwd);
}

/*
**
**
**
*/
void builtin_cd(char **args)
{
	char *str;
	if(args && args[0] && args[1])
		handle_error(ERR_MSG_ARGS, NULL, ERR_CD);
	else if (!args)
	{
		str = p_find_node_by_name(sget_env(NULL),"HOME");
		if (!str && (free(str),1))	
			handle_error(ERR_MSG_CD_HOME, NULL, ERR_CD);
	}
	else if (strcmp(args[0], "-") == 0) //replace with ft_strcmp
	{
		str = p_find_node_by_name(sget_env(NULL),"OLDPWD");
		if (!str && (free(str),1))	
			handle_error(ERR_MSG_CD_HOME, NULL, ERR_CD);
	}
	else
		str = args[0];
	if(chdir(str) == -1)
		printf("%s\n", args[0]);
}