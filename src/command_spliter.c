/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_spliter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 20:07:47 by wayden            #+#    #+#             */
/*   Updated: 2023/11/05 20:01:27 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_pipe(char c)
{
	return(c == '|');
}

void check_special(int *index, char *str)
{
	int i;

	i = *index;	
	if(is_quotes(str[i]))
	{
		if(check_closed(&i, str[i]))
		{
			*index = i;
			return;
		}
		else
			i++;
	}	
	


	
}



int count_cmd(char *str)
{
	int i;
	int cmds;
	
	i = 0;
	cmds = 0;
	while(str[i])
	{
		while (str[i] && is_pipe(str[i]))
			i++;
		if(str[i])
			cmds++;
		while(str[i] && !is_pipe(str[i]))
			check_special(&i, str);
	}
	return (cmds);
}

int is_space(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}

int count_words(char *str)
{
	int i;
	int words;

	i = 0;
	words = 0;
	while (str[i])
	{
		while (str[i] && is_space(str[i]))
			i++;
		if (str[i])
			words++;
		while (str[i] && !is_space(str[i]))
			i++;
	}
	return (words);
}

char **ft_split(char *str)
{
	int i;
	int j;
	int k;
	int words;
	char **tab;

	i = 0;
	j = 0;
	k = 0;
	words = count_words(str);
	tab = (char **)malloc(sizeof(char *) * words);
	while (str[i])
	{
		while (str[i] && is_space(str[i]))
			i++;
		j = i;
		while (str[i] && !is_space(str[i]))
			i++;
		if (i > j)
			tab[k] = (char *)malloc(sizeof(char) * ((i - j) + 1));
		if (i > j)
			tab[k] = ft_strncpy(tab[k++], &str[j], i - j);
	}
	tab[k] = NULL;
	return (tab);
}

t_cmd **split_commands(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
	}
}