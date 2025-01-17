#include "get_next_line_bonus.h"

char *find_line(char *s)
{
    char *str;
    size_t i;

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

char *combine_leftover_and_buffer(char *leftover, char *buffer)
{
    char *temp;

	if (!leftover && !buffer)
        return (NULL);
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
        if (!*leftover || bytes_read == 0 || ft_strchr(buffer, '\n'))
            break;
    }
    free(buffer);
	if (bytes_read == 0 && (!*leftover || **leftover == '\0'))
        return (NULL);
    return (*leftover);
}

char	*get_next_line(int fd)
{
    static char *leftover[MAX_FD];
    char *line;
    char *temp;

    if (fd < 0 || fd >= MAX_FD)
        return (NULL);
    if (!read_and_update_leftover(fd, &leftover[fd]))
        return (free(leftover[fd]), NULL);
    line = find_line(leftover[fd]);
    if (line)
    {
        temp = leftover[fd];
        leftover[fd] = ft_strdup(leftover[fd] + ft_strlen(line));
        free(temp);
        return (line);
    }
    line = leftover[fd];
    leftover[fd] = NULL;
    return (line);
}

/*
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd1 = open("text1.txt", O_RDONLY);
    int fd2 = open("text2.txt", O_RDONLY);
    char *line;

    if (fd1 < 0 || fd2 < 0)
    {
        perror("Error opening files");
        return (1);
    }

    printf("Reading from fd1:\n");
    line = get_next_line(fd1);
    printf("%s", line);
    free(line);

    printf("\nReading from fd2:\n");
    line = get_next_line(fd2);
    printf("%s", line);
    free(line);

    printf("\nReading from fd1 again:\n");
    line = get_next_line(fd1);
    printf("%s", line);
    free(line);

    close(fd1);
    close(fd2);

    return (0);
}
*/