/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: therodri <therodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 21:05:56 by therodri          #+#    #+#             */
/*   Updated: 2022/12/10 22:43:21 by therodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Écrit le caractère ’c’ sur le descripteur de
fichier donné.
*/
#include "../../header/libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
