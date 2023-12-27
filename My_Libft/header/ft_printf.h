/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 23:10:37 by therodri          #+#    #+#             */
/*   Updated: 2023/12/27 05:33:15 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "libft.h"
# include <limits.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define CONVERT "cspdiuxX%"
# define FLAGS "0-.+# "
# define BONUS 1
# define HEX "0123456789abcdef"
# define BEX "0123456789ABCDEF"
# define TEN "0123456789"

typedef unsigned long long int	t_mem;

typedef struct s_args
{
	int		conv;
	int		n[2];
	int		flags[10];
	char	type;
	char	*base;
	t_mem	t_base;
	int		count;
	int		i;
	int		pre_z;
}			t_args;

typedef struct s_var
{
	size_t	i;
	int		total_count;
	t_mem	va_args;
}			t_var;

/*----ft_pf_simpleft.c--*/
char		ft_strchr_index(const char *s, int c);
t_mem		ft_pf_check_null(t_mem n, t_args *args);
t_mem		ft_pf_abs(long int argslist, t_args *args);
/*----ft_pf_print.c--*/
void		ft_print_sign(t_args *args, int *total_count, t_mem arguments);
void		ft_putnbr_base_f(t_mem n, t_args *args, int *total_count);
void		ft_putstr_f(t_mem va_args, t_args *args, int *total_count);
void		ft_print(t_mem arguments, t_args *args, int *total_count);
/*----ft_pf_count.c--*/
void		ft_count_hub(t_mem arguments, t_args *args);
void		ft_putnbr_base_c(t_mem n, t_args *args);
void		ft_putstr_c(t_mem va_args, t_args *args);
void		ft_sign_field(t_args *args, t_mem arguments);
/*----ft_printf.c--*/
void		ft_convert_hub(t_mem arguments, t_args *args, int *total_count);
void		ft_init_struct(t_args *args);
t_mem		ft_init_args(va_list argslist, t_args *args, int *total_count);
void		ft_init_bal(const char *str, t_args *args);
int			ft_printf(char const *str, ...);

#endif
