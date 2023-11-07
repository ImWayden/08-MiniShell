/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 21:52:28 by wayden            #+#    #+#             */
/*   Updated: 2023/11/07 23:23:02 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void env_delone(t_env *node) {
    if (node) {
        free(node->name);
        free(node->content);
        free(node->full);
        free(node);
    }
}

void env_remove_if(t_env **begin_list, char *name, int (*cmp)())
{
	t_env *cur = *begin_list;

	if (begin_list == NULL || *begin_list == NULL)
		return;
	
	if (!cmp(cur->name, name))
	{
		*begin_list = cur->next;
		env_delone(cur);
		env_remove_if(begin_list, name, cmp);
	}
	cur = *begin_list;
	env_remove_if(&cur->next, name, cmp);
}



static t_env *env_last(t_env *env)
{
	while (env && env->next)
		env = env->next;
	return (env);
}

void env_add_back(t_env **env, t_env *new)
{
	if (!*env)
	{
		*env = new;
		return;
	}
	if (*env && env && new)
		env_last(*env)->next = new;
}

t_env *env_new(char *str)
{
	t_env *new_var;
	int i;

	i = 0;
	new_var = (t_env *)malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	while (str[i] && str[i] != '=')
		i++;
	new_var->name = (char *)malloc(sizeof(char) * (i + 1));
	ft_strncpy(new_var->name, str, i);
	if (str[i])
		new_var->content = ft_strdup(&str[i + 1]);
	else
		return (free(new_var->name), free(new_var), NULL);
	new_var->next = NULL;
	new_var->full = ft_strdup(str);
	return (new_var);
}

t_env **sget_env(char **envp)
{
	static t_env *env = NULL; // Utilisez un simple pointeur ici
	static bool initialized = FALSE;
	int i;

	if (!initialized)
	{
		i = -1;
		env = NULL; // Initialisation de env à NULL
		while (envp[++i])
			env_add_back(&env, env_new(envp[i])); // Passer un pointeur vers env
		initialized = TRUE;
	}

	return &env; // Retourner un pointeur vers env
}

t_env *find_node_by_name(t_env **beign_list, const char *name_to_find) {
    t_env *current = *beign_list;

    while (current != NULL) {
        if (strcmp(current->name, name_to_find) == 0) {
            // La chaîne a été trouvée, renvoyer la node correspondante
            return current;
        }
        current = current->next;
    }

    // La chaîne n'a pas été trouvée, renvoyer NULL
    return NULL;
}

int main(int argc, char *argv[], char **envp)
{

	char *input;

	sget_input();
	t_token **tokens = sget_token();
	t_env *env = *sget_env(envp); // Initialisation de la liste chaînée
	builtin_export("bonjour=");
	builtin_export("bonjour2");
	builtin_export("bonjour3=");
	char **str_array = sget_env_tab(NOP);
	for (int i = 0; str_array[i] != NULL; i++) {
        printf("Index %d : %s\n", i, str_array[i]);
    }
	display_token_list(*tokens);
	builtin_env(env);
	clean_env();
	return 0;
}