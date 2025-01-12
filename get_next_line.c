#include "get_next_line.h"
#include <stdio.h>

char *find_line(char *s, int c)
{
    char *str;
    size_t i;

    i = 0;
    while (s[i] && s[i] != (char)c)
        i++;
    if (s[i] == (char)c)
        i++;
    str = (char *)malloc((i + 1) * sizeof(char));
    if (!str)
        return (ft_strdup(s));
    ft_strlcpy(str, s, i + 1);
    return (str);
}

char *combine_leftover_and_buffer(char *leftover, char *buffer)
{
    char *temp;

    if (!leftover)
        return (ft_strdup(buffer));
    temp = leftover;
    leftover = ft_strjoin(leftover, buffer);
    free(temp);
    return (leftover);
}

char *read_and_update_leftover(int fd, char **leftover)
{
    char *buffer;
    ssize_t bytes_read;

    buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
    if (!buffer)
        return (NULL);
    while (1)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1)
            return (free(buffer), NULL);
        buffer[bytes_read] = '\0';
        *leftover = combine_leftover_and_buffer(*leftover, buffer);
        if (bytes_read == 0 || ft_strchr(buffer, '\n'))
            break;
    }
    free(buffer);
	if (bytes_read == 0 && (!*leftover || **leftover == '\0'))
        return (NULL);
    return (*leftover);
}

char	*get_next_line(int fd)
{
    static char *leftover;
    char *line;
    char *temp;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    if (!read_and_update_leftover(fd, &leftover))
        return (NULL);
    line = find_line(leftover, '\n');
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

#include <fcntl.h>

int main(void)
{
    int fd = open("text.txt", O_RDONLY);
    char *line;

    if (fd < 0)
    {
        perror("Error opening file");
        return (1);
    }

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    close(fd);
    return (0);
}
