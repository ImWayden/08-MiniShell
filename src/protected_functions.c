/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protected_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 04:39:18 by wayden            #+#    #+#             */
/*   Updated: 2023/11/15 02:25:50 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int p_open(const char *file, int flags)
{
	int ret;
	
	ret = open(file, flags);
	if (ret == -1)
		handle_error(ERR_MSG_OPEN, file, ERR_OPEN);
	return(ret);
}

int p_access(const char *file, int flags)
{
	int ret;

	if(flags & F_OK && access(file, F_OK) == -1)
		handle_error(ERR_MSG_ACCESS_EXIST, file, ERR_ACCESS);
	if(flags & W_OK && access(file, W_OK) == -1)
		handle_error(ERR_MSG_ACCESS, file, ERR_ACCESS);
	if(flags & R_OK && access(file, R_OK) == -1)
		handle_error(ERR_MSG_ACCESS, file, ERR_ACCESS);
	return (0);
}

void *p_malloc(size_t size)
{
	void *data;
	data = malloc(size);
	if (data == NULL)
		handle_error(ERR_MSG_MALLOC, NULL, ERR_MALLOC);
	return (data);
}

void p_free(void **ptr) 
{
    if (*ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    } else {
		printf("Tentative de double free détectée.");
        // Gestion de l'erreur (double free détecté)
        //handle_error("Tentative de double free détectée.", NULL, ERR_DOUBLE_FREE);
	}
}