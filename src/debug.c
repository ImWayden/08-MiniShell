/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 01:46:04 by wayden            #+#    #+#             */
/*   Updated: 2023/12/30 19:28:08 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**
** testing parts
** 
*/
void print_cmd(t_cmd *cmd) {
    if (cmd == NULL) {
        printf("Structure de commande NULL\n");
        return;
    }
	t_cmd current;
	current = cmd[0];
	int i = 0;
	int nb_cmd = cmd[0].nb_cmd;

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
		// Vous pouvez également appeler récursivement print_cmd pour afficher la structure suivante
}
/*
** print scmd
*/
void print_scmd(t_scmd *cmd_struct) {
    if (cmd_struct == NULL) {
        printf("Structure non initialisée.\n");
        return;
    }

    printf("Commande : %s\n", cmd_struct->cmd);

    printf("Arguments : ");
    char **args = cmd_struct->args;
    while (*args != NULL) {
        printf("%s ", *args);
        args++;
    }
    printf("\n");

    printf("Is_builtin : %d\n", cmd_struct->is_builtin);
}

const char *token_type_to_string(t_token_type type)
{
	switch (type)
	{
	case TK_HEREDOC:
		return "TK_HEREDOC";
	case TK_CONCAT:
		return "TK_CONCAT";
	case TK_REDIR_ENT:
		return "TK_REDIR_ENT";
	case TK_REDIR_EXT:
		return "TK_REDIR_EXT";
	case TK_SPACE:
		return "TK_SPACE";
	case TK_PIPE:
		return "TK_PIPE";
	case TK_SQUOTE:
		return "TK_SQUOTE";
	case TK_DQUOTE:
		return "TK_DQUOTE";
	case TK_NOTOKEN:
		return "TK_NOTOKEN";
	case TK_WORD:
		return "TK_WORD";
	default:
		return "Unknown";
	}
}

// Fonction pour afficher une liste chaînée de tokens avec des noms de token en chaîne
void display_token_list(t_token *token_list)
{
	t_token *current = token_list;
	while (current)
	{
		printf("Content: %s, Size: %ld, Token Type: %s\n", current->content, current->size, token_type_to_string(current->type));
		current = current->next;
	}
}