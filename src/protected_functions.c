/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protected_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 04:39:18 by wayden            #+#    #+#             */
/*   Updated: 2024/01/01 21:38:47 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*p_find_node_by_name(t_env **beign_list, const char *name_to_find)
{
	t_env	*env;

	env = find_node_by_name(beign_list, name_to_find);
	if (env)
		return (env->content);
	else
		return (ft_strdup_gc("", 1));
}

int	p_open(const char *file, int flags, mode_t rights)
{
	int	ret;

	ret = open(file, flags, rights);
	if (ret == -1)
		handle_error(ERR_MSG_OPEN, file, ERR_OPEN);
	return (ret);
}

int	p_access(const char *file, int flags)
{
	if (flags & F_OK && access(file, F_OK) == -1)
		handle_error(ERR_MSG_ACCESS_EXIST, file, ERR_ACCESS);
	if (flags & W_OK && access(file, W_OK) == -1)
		handle_error(ERR_MSG_ACCESS, file, ERR_ACCESS);
	if (flags & R_OK && access(file, R_OK) == -1)
		handle_error(ERR_MSG_ACCESS, file, ERR_ACCESS);
	return (0);
}

void	*p_malloc(size_t size, bool is_tmp)
{
	void	*data;

	data = malloc_gc(size, is_tmp);
	if (data == NULL)
		handle_error(ERR_MSG_MALLOC, NULL, ERR_MALLOC);
	return (data);
}

void	p_free(void **ptr)
{
	if (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
}
