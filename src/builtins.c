/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:15:36 by wayden            #+#    #+#             */
/*   Updated: 2023/11/22 07:32:48 by wayden           ###   ########.fr       */
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
    while (env != NULL) {
        printf("%s=%s\n", env->name, env->content);
        env = env->next;
    }
}

/*
** 
** 	fonctionne a priori peut etre beosin de plus de tests
**  ne fonctionne pas dans les commande piped 
** 
*/
void builtin_export(char *str) // a remplacer par arg1 trouvé dans le parsing de la commande
{
		env_add_back(sget_env(NULL), env_new(str));
		refresh_env_tab();
}

/*
**	should work flawlessly
**	just besoin de changer strcmp en ft_strcmp 
**	que je n'ai malheureusement pas dans ma libft
*/
void builtin_unset(char *name)
{
	env_remove_if(sget_env(NULL), name, strcmp);
	refresh_env_tab();
}

/*
** executable si la commande contient des pipes
** 
**
*/
void builtin_echo(char **strs, int flag_n)
{
	int i;

	i = 0;
	while(strs && strs[i] != NULL)
	{
		write(STDOUT_FILENO, strs[i], ft_strlen(strs[i]));
		i++;
		if(strs[i] != NULL)
			write(STDOUT_FILENO, " ", 1);
	}
	if(!flag_n)
		write(STDOUT_FILENO, "\n", 1);
}

/*
** a priori non executable 
** si la commande contient des pipes
**
*/
void builtin_exit(void)
{
	clean_all();
	exit(0);
	kill(ft_atoi(getenv("$$")),SIGQUIT);
}
/*
** builtin pwd executable dans ligne
** de commadne contenant des pipes
**
*/
void builtin_pwd(void)
{
	char *cwd = getcwd(NULL, 0);
    if (cwd == NULL) {
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
void builtin_cd(const char *path)
{
	if ((path[0] != '/' && path[0] != '.') || (chdir(path) == -1))
        perror("cd");
}