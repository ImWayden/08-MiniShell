/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_split_gc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:41:57 by olcoste           #+#    #+#             */
/*   Updated: 2023/12/27 05:33:45 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./garbage_collector.h"

static int	nb_word_gc(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	if (!str)
		return (0);
	count = 0;
	while (str[i])
	{
		if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static char	**ft_str_to_tab_gc(char *str, char c, char **res, bool is_tmp)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (j < nb_word_gc(str, c))
	{
		k = 0;
		while (str[i] == c)
			i++;
		i--;
		while (str[++i] != c && str[i])
			k++;
		if (k)
		{
			res[j] = ft_substr_gc(str, i - k, k, is_tmp);
			if (!res[j])
				return (exit_error_gc());
			j++;
		}
		i++;
	}
	return (res);
}

char	**ft_split_gc(const char *str, char c, bool is_tmp)
{
	char	**res;
	int		count;

	count = nb_word_gc((char *)str, c);
	res = (char **)ft_calloc_gc(sizeof(char *), count + 1, is_tmp);
	if (!res || !str)
		return (NULL);
	if (str[0] == 0 && c == 0)
	{
		res[0] = NULL;
		return (res);
	}
	res[count] = NULL;
	if (!ft_str_to_tab_gc((char *)str, c, res, is_tmp))
		return (exit_error_gc());
	return (res);
}
