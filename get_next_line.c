/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zsalih <zsalih@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 13:21:06 by zsalih            #+#    #+#             */
/*   Updated: 2025/01/18 21:06:35 by zsalih           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*find_line(char *s)
{
	char	*str;
	size_t	i;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		i++;
	str = (char *)malloc((i + 1) * sizeof(char));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s, i + 1);
	return (str);
}

char	*combine_leftover_and_buffer(char *leftover, char *buffer)
{
	char	*temp;

	if (!leftover)
		return (ft_strdup(buffer));
	if (!buffer)
		return (leftover);
	temp = leftover;
	leftover = ft_strjoin(leftover, buffer);
	free(temp);
	return (leftover);
}

char	*read_and_update_leftover(int fd, char **leftover)
{
	char	*buffer;
	ssize_t	bytes_read;

	if (BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc((BUFFER_SIZE + 1U) * sizeof(char));
	if (!buffer)
		return (NULL);
	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(buffer), NULL);
		buffer[bytes_read] = '\0';
		*leftover = combine_leftover_and_buffer(*leftover, buffer);
		if (bytes_read == 0 || ft_strchr(buffer, '\n') || !*leftover)
			break ;
	}
	free(buffer);
	if (bytes_read == 0 && (!*leftover || **leftover == '\0'))
		return (NULL);
	return (*leftover);
}

char	*get_next_line(int fd)
{
	static char	*leftover;
	char		*line;
	char		*temp;

	if (fd < 0)
		return (NULL);
	if (!read_and_update_leftover(fd, &leftover))
	{
		free(leftover);
		leftover = NULL;
		return (NULL);
	}
	line = find_line(leftover);
	if (line)
	{
		temp = leftover;
		leftover = ft_strdup(leftover + ft_strlen(line));
		free(temp);
		return (line);
	}
	line = leftover;
	leftover = NULL;
	return (line);
}

// #include <fcntl.h>
// #include <stdio.h>

// int	main(void)
// {
// 	int		fd;
// 	char	*line;

// 	fd = open("text1.txt", O_RDONLY);
// 	if (fd < 0)
// 		return (1);
// 	line = get_next_line(fd);
// 	printf("%s", line);
// 	free(line);
// 	line = get_next_line(fd);
// 	printf("%s", line);
// 	free(line);
// 	close(fd);
// 	return (0);
// }
