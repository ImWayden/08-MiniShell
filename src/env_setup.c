/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 21:52:28 by wayden            #+#    #+#             */
/*   Updated: 2023/11/15 04:27:47 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void env_delone(t_env *node)
{
	if (node)
	{
		p_free((void **)&node->name);
		p_free((void **)&node->content);
		p_free((void **)&node->full);
		p_free((void **)&node);
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
	new_var = (t_env *)p_malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	while (str[i] && str[i] != '=')
		i++;
	new_var->name = (char *)p_malloc(sizeof(char) * (i + 1));
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
	static t_env *env = NULL;
	static bool initialized = FALSE;
	int i;

	if (!initialized)
	{
		i = -1;
		env = NULL;
		while (envp[++i])
			env_add_back(&env, env_new(envp[i]));
		initialized = TRUE;
	}

	return &env;
}

t_env *find_node_by_name(t_env **beign_list, const char *name_to_find)
{
	t_env *current = *beign_list;

	while (current != NULL)
	{
		if (strcmp(current->name, name_to_find) == 0)
			return current;
		current = current->next;
	}
	return NULL;
}


void print_cmd(t_cmd *cmd) {
    if (cmd == NULL) {
        printf("Structure de commande NULL\n");
        return;
    }
	t_cmd current;
	current = cmd[0];
	int i = 0;
	int nb_cmd = cmd[0].nb_cmd;
	while(i < nb_cmd)
	{
		printf("Commande: %s\n", current.cmd);
		printf("Arguments:\n");
		char **arg = current.args;
		while (arg && *arg != NULL) {
			printf("  %s\n", *arg);
			arg++;
		}
		printf("Entrée: %s\n", current.input);
		printf("Sortie: %s\n", current.output);
		printf("Here_doc: %s\n", current.here_doc);
		printf("Concat: %s\n", current.concat);
		printf("Builtin: %d\n", current.is_builtin);
		i++;
		if(i < nb_cmd)
			current = cmd[i];
	}
		// Vous pouvez également appeler récursivement print_cmd pour afficher la structure suivante

}


int main(int argc, char *argv[], char **envp)
{

	char *input;

	sget_init(0, REFRESH);
	sget_input();
	t_env *env = *sget_env(envp);
	sget_location_flag(ERR_ENV);
	t_token **tokens = sget_token();
	sget_location_flag(ERR_TOKEN);
	builtin_export("bonjour=");
	builtin_export("bonjour2");
	builtin_export("bonjour3=");
	char **str_array = sget_env_tab(NOP);
	printf("%s\n", sget_input());
	// for (int i = 0; str_array[i] != NULL; i++) {
	//     printf("Index %d : %s\n", i, str_array[i]);
	// }
	print_cmd(sget_cmd_tab());
	sget_location_flag(ERR_PARSER);
	display_token_list(*tokens);
	// builtin_env(env);
	clean_cmds();
	clean_tokens();
	clean_env();
	return 0;
}