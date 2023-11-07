/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singletons.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 03:35:29 by wayden            #+#    #+#             */
/*   Updated: 2023/11/07 03:58:33 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *sget_input()
{
	static char *input;

	if(!sget_init(INPUT, NOP) && sget_init(INPUT, SET))
		input = readline(RED "minishell:>" RESET);
	return (input);
}