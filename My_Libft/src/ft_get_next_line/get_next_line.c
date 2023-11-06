/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: therodri <therodri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 02:58:04 by therodri          #+#    #+#             */
/*   Updated: 2022/12/10 23:29:54 by therodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/get_next_line.h"

void	ft_buffer_stock(t_line *buf, char *buffer, int n)
{
	char	*tmp;

	tmp = buf->line;
	buf->line = ft_calloc(buf->len + n + 1, sizeof(char));
	ft_memmove(buf->line, tmp, buf->len);
	ft_memmove(&buf->line[buf->len], buffer, n);
	buf->len += n;
	free(tmp);
	if (n < BUFFER_SIZE)
	{
		ft_memmove(buffer, &buffer[n], BUFFER_SIZE - n);
		ft_memset(&buffer[BUFFER_SIZE - n], 0, n);
		ft_memset(&buf->line[buf->len], 0, 1);
	}
	else
		ft_memset(buffer, 0, BUFFER_SIZE);
}

int	ft_fill_buff(t_line *buf, int fd, char *buffer)
{
	if (buf->line == NULL)
		return (0);
	buf->i = 0;
	while (buf->i < buf->end && buffer[buf->i] != '\n')
	{
		if (buf->i == BUFFER_SIZE - 1)
		{
			ft_buffer_stock(buf, buffer, BUFFER_SIZE);
			buf->end = read(fd, buffer, BUFFER_SIZE);
			if (buf->end < 0)
				return (1);
			buf->i = -1;
		}
		buf->i++;
	}
	if (buffer[buf->i] == '\n')
		buf->end = buf->i + 1;
	ft_buffer_stock(buf, buffer, buf->end);
	return (0);
}

int	ft_init_line(t_line *buf, char buffer[BUFFER_SIZE + 1])
{
	int	i;

	i = 0;
	buf->len = 0;
	while (buffer && buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer || !*buffer)
		buf->line = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	else
	{
		if (buffer[i] == '\n')
			i++;
		ft_buffer_stock(buf, buffer, i);
		if (!buf->line || buf->line[i - 1] == '\n')
			return (1);
	}
	return (0);
}

char	*get_next_line(int fd)
{
	t_line		buf;
	static char	buffer[BUFFER_SIZE + 1];

	buf.line = NULL;
	if (ft_init_line(&buf, buffer))
		return (buf.line);
	if ((fd < 1 && !buf.line))
		return (NULL);
	buf.end = read(fd, buffer, BUFFER_SIZE);
	if (buf.end < 0 && !buf.line)
		return (NULL);
	if (buf.end >= 1 || !buf.line)
		if (ft_fill_buff(&buf, fd, buffer))
			return (free(buf.line), NULL);
	if ((!buf.line || !*buf.line) && !*buffer)
		return (free(buf.line), NULL);
	if (!buf.line || !*buf.line)
		return (NULL);
	return (buf.line);
}
