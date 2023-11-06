/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: therodri <therodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 18:53:42 by therodri          #+#    #+#             */
/*   Updated: 2022/12/10 22:42:50 by therodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
The bzero() function erases the data in the n bytes of the memory 
starting at the location pointed to by s,
by writing zeros (bytes containing '\0') to that area.
*/
#include "../../header/libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*str;

	str = (char *)s;
	while (n-- > 0)
		*(str++) = 0;
}
