/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 00:15:36 by wayden            #+#    #+#             */
/*   Updated: 2023/11/07 03:20:37 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



/*
** fonctionne bien.
**
**
*/
void builtin_env(t_env *env){
    while (env != NULL) {
        printf("%s=%s\n", env->name, env->content);
        env = env->next;
    }
}

/*
** a gérer dans le parsing des commandes je dois récup l'arg 1
** arg 1 est une chaine de caractère sous forme VARNAME=VALUE
** ce qui est parfait et peut etre ajouté au path en utilisant
** les fonctions déja faites dans getenv
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