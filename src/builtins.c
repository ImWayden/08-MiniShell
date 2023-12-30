/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:15:36 by wayden            #+#    #+#             */
/*   Updated: 2023/12/30 21:28:49 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	fonctionne bien a priori.
**	a priori pas besoin de quitter le programme en cour
**
*/
void	builtin_env(t_env *env)
{
	while (env != NULL)
	{
		if (env->isok == TRUE)
		{
			ft_putstr_fd(env->name, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(env->content, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		env = env->next;
	}
}

/*
** executable si la commande contient des pipes
**
**
*/
void	builtin_echo(char **args, int flag_n)
{
	int	i;
	int	j;

	flag_n = 0;
	i = 1;
	j = 2;
	while (args && args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		while (args[i][j] == 'n')
			j++;
		if (!args[i][j])
		{
			flag_n = 1;
			i++;
		}
		else
			break ;
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
void	builtin_exit(char **args)
{
	int	*exit_code;
	int tmp;
	exit_code = sget_exitcode();

	if (args && args[0])
	{
		tmp = ft_simple_atoi_error(args[0]);
		if (tmp == RETURN_EXIT_NUM_ERR)
		{
			*sget_exitcode() = 2;
			handle_error(ERR_MSG_ARG_LET, "exit", ERR_EXIT_LET);
		}
		else if (!(args && args[0] && args[1]) && *exit_code == RETURN_EXECBACK)
			*exit_code = tmp;
	}
	if (args && args[0] && args[1])
		return (handle_error(ERR_MSG_ARGS_NUM, "exit", ERR_EXIT_NUM), (void)0);
	clean_all();
	free_all_garbage();
	exit(*exit_code);
}

/*
** builtin pwd executable dans ligne
** de commadne contenant des pipes
**
*/
void	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	add_tmp_garbage(cwd);
	if (cwd == NULL)
	{
		if (errno == ENOENT)
			printf("Error: Current \
working directory does not exist.\n");
		else
			perror("getcwd");
		close(STDIN_FILENO);
		free_all_garbage();
		exit(EXIT_FAILURE);
	}
	printf("%s\n", cwd);
	close(STDIN_FILENO);
}

/*
**
**
**
*/
void	builtin_cd(char **args)
{
	char	*str;

	if (args && args[0] && args[1])
		handle_error(ERR_MSG_ARGS_NUM, NULL, ERR_CD);
	else if (!args)
	{
		str = p_find_node_by_name(sget_env(NULL), "HOME");
		if (!str[0])
			return (handle_error(ERR_MSG_CD_HOME, NULL, ERR_CD), (void)0);
	}
	else if (ft_strcmp(args[0], "-") == 0)
	{
		str = p_find_node_by_name(sget_env(NULL), "OLDPWD");
		if (!str[0])
			return (handle_error(ERR_MSG_CD_HOME, NULL, ERR_CD), (void)0);
	}
	else
		str = args[0];
	if (chdir(str) == -1)
		return (handle_error(ERR_MSG_CD, str, ERR_CD), (void)0);
	else
		*sget_exitcode() = 0;
}
