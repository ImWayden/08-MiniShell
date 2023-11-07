/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 22:56:43 by wayden            #+#    #+#             */
/*   Updated: 2023/11/07 04:38:10 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




static t_token *token_last(t_env *env)
{
	while (env && env->next)
		env = env->next;
	return (env);
}




void token_add_back(t_token **token, t_token *new)
{
	t_token *prev;
	if (!*token)
	{
		*token = new;
		return;
	}
	
	if (*token && token && new)
	{
		prev = token_last(*token);
		prev->next = new;
		new->prev = prev;
	}
}










t_token *token_new(t_token *new)
{
	t_token *new_token = (t_token *)malloc(sizeof(t_token));
    
    // Copier les données de la structure new dans le nouveau nœud
    new_token->content = strdup(new->content);
    new_token->size = new->size;
    new_token->token_type = new->token_type;
    free(new->content);
    // Ajouter le nouveau nœud à la liste chaînée
    new_token->next = NULL;
}

void token_add(t_token **tk_lst, t_token *new)
{
	token_add_back(tk_lst, token_new(new));
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



bool is_quote(int *index, char *str, int *token)
{
	int i;

	i = *index;
	if (str[i] == '\'' || str[i] == '\"')
	{
		while (str[i] && str[i] != "\'" && str[i] != "\"")
			i++;
		if (str[i] && str[i] != "\'")
			return (*token = TK_SQUOTE, *index = i, TRUE);
		else if (str[i] && str[i] != "\'")
			return (*token = TK_DQUOTE, *index = i, TRUE);
		else
			return (FALSE);
	}
	return (FALSE);
}


void here_doc(int *index, char *str, int *token)
{
	int i;

	i = *index;
	while(!is_space(str[i]))
		i++;


	
}


bool is_redir(int *index, char *str, int *token)
{
	int i;

	i = *index;
	if (str[i] == '<')
	{
		if(str[i + 1] && str[i + 1] == '<')
			return(here_doc(index,str,&token),TRUE);
		else
			return(*token = TK_REDIR_ENT, *index = i, TRUE);
	}
	if (str[i] == '>')
	{
		if(str[i + 1] && str[i + 1] == '>')
			return(concat(index,str,&token),TRUE);
		else
			return(*token = TK_REDIR_EXT, *index = i, TRUE);
	}
}

void tokenisateur(t_token **tk_lst, char *str)
{
	int i;
	int k;
	int token;

	i = 0;
	k = 0;
	while (str[i])
	{

		token = TK_NOTOKEN;
		if (is_quote(&i, str, &token) || is_special(&i, str, &token) 
				|| is_pipe(&i, str, &token) || is_space(str[i]))
		{
			token_add(tk_lst, (t_token ){ft_substr(str, k, i - k), i - k, token});
			k = i + 1;
		}
		else if (str[i])
			i++;
	}
}

t_cmd **sget_cmd_tab()
{
	static t_cmd	**cmd;
	size_t			i;
	size_t			nb_cmd;

	if (!sget_init(CMD, NOP) && sget_init(CMD, SET))
	{
		i = -1;
		nb_cmd = get_nb_cmd(sget_token());// traverse les tokens compte le nombre de token_pipe;
		cmd = (t_cmd **)malloc(sizeof(t_cmd *) * nb_cmd);
		while(++i < nb_cmd)
			setup_cmd(cmd[i], sget_token());
	}
	return (cmd);
}

t_token **sget_token()
{
	static t_token *token_list = NULL;

	if(!sget_init(TOKEN, NOP) && sget_init(TOKEN, SET))
	{
		token_list = NULL;
		tokenisateur(&token_list, sget_input(FALSE));
	}
	return(&token_list);
}

bool sget_init(t_init index, bool set)
{
	static bool init_tab[5] = {FALSE, FALSE, FALSE, FALSE, FALSE};
	if(set == REFRESH)
		init_tab[index] = FALSE;
	else if(set == SET)
		init_tab[index] = TRUE;
	return(init_tab[index]);
}