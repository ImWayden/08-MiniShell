/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_str_gc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:35:47 by olcoste           #+#    #+#             */
/*   Updated: 2024/01/01 22:18:09 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./garbage_collector.h"

char	*ft_strjoin_gc(char const *s1, char const *s2, bool is_tmp)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	add_garbage(res, is_tmp);
	return (res);
}

char	*ft_strdup_gc(const char *s, bool is_tmp)
{
	char	*res;

	res = ft_strdup(s);
	add_garbage(res, is_tmp);
	return (res);
}

char	*ft_substr_gc(char const *s, unsigned int start, size_t len, bool tmp)
{
	char	*res;

	res = ft_substr(s, start, len);
	add_garbage(res, tmp);
	return (res);
}
