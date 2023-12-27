/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 13:02:16 by olcoste           #+#    #+#             */
/*   Updated: 2023/12/27 05:29:09 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

# include "../../minishell.h"

typedef struct s_garbage
{
	t_list				*tmp_lst;
	t_list				*close_lst;
	struct s_garbage	*next;
}	t_garbage;

t_garbage	*get_head_garbage(void);
t_list		**get_tmp_garbage(void);
t_list		**get_close_garbage(void);

int			add_tmp_garbage(void *ptr);
int			add_close_garbage(void *ptr);
void		add_garbage(void *ptr, bool is_tmp);

void		free_tmp_garbage(void);
void		free_all_garbage(void);
void		*exit_error_gc(void);
// LIBFT_GC

void		*malloc_gc(size_t size, bool is_tmp);
void		*ft_calloc_gc(size_t nmemb, size_t size, bool is_tmp);
char		*ft_itoa_gc(int n, bool is_tmp);

char		*ft_strjoin_gc(char const *s1, char const *s2, bool is_tmp);
char		*ft_strdup_gc(const char *s, bool is_tmp);
char		*ft_substr_gc(char const *s, unsigned int start, size_t len,
				bool tmp);

char		**ft_split_gc(const char *str, char c, bool is_tmp);

//PAS UTILISEE
// t_list	*ft_lstnew(void *content);
// t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
// char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
// char	*ft_strtrim(char const *s1, char const *set);

#endif