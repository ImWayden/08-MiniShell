/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_gc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olcoste <olcoste@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:02:09 by olcoste           #+#    #+#             */
/*   Updated: 2023/12/26 14:26:05 by olcoste          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./garbage_collector.h"

void	*malloc_gc(size_t size, bool is_tmp)
{
	void	*res;

	res = malloc(size);
	add_garbage(res, is_tmp);
	return (res);
}

void	*ft_calloc_gc(size_t nmemb, size_t size, bool is_tmp)
{
	void	*res;

	res = ft_calloc(nmemb, size);
	add_garbage(res, is_tmp);
	return (res);
}

char	*ft_itoa_gc(int n, bool is_tmp)
{
	char	*res;

	res = ft_itoa(n);
	add_garbage(res, is_tmp);
	return (res);
}
