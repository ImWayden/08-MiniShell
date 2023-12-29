/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 00:00:51 by therodri          #+#    #+#             */
/*   Updated: 2023/12/28 04:04:14 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Alloue (avec malloc(3)) et retourne une chaîne de
caractères issue de la chaîne ’s’.
Cette nouvelle chaîne commence à l’index ’start’ et
a pour taille maximale ’len’.
*/
#include "../../header/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	int		i;
	char	*str;
	size_t	size;

	size = ft_strlen(s);
	i = 0;
	if (start > size)
		return (ft_strdup(""));
	if (len > size - start)
		len = size - start;
	str = ft_calloc(sizeof(char), (len + 1));
	if (!str)
		return (NULL);
	while (len-- > 0 && s[start])
		str[i++] = s[start++];
	return (str);
}
