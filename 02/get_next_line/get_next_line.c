#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>

int	ft_strlen(char *str)
{
	int i;

	i  = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char *ft_strjoin(char *s1, char *s2)
{
	char *new;
	int i;
	int j;
	int len_new;

	i = 0;
	j = 0;
	if (!s1 && !s2)
		return (malloc(sizeof(char)));
	len_new = ft_strlen(s1) + ft_strlen(s2);
	new = malloc(sizeof(char) * (len_new + 1));
	*new = 0;
	while (s1 && s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] && s2[j])
	{
		new[i] = s2[j];
		i++;
		j++;
	}
	new[i] = 0;
	free(s1);
	return (new);
}

char *ft_substr(char *s, int start, int len)
{
	char *new;
	int i;

	i = 0;
	new = malloc(sizeof(char) * len + 1);
	if (!s || !new)
		return (new);
	if (start > ft_strlen(s))
		return (new);
	*new = 0;
	while (s && s[start] && i < len)
	{
		new[i] = s[start];
		start++;
		i++;
	}
	new[i] = 0;
	return (new);
}

int	findchar(char *str, char c)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int firstline(char **save, char **line)
{
	char *tmp;
	int i;

	i = 0;
	if (**save == 0)
	{
		if (*line)
			free(*line);
		*line = NULL;
		return (0);
	}
	while (*(*save + i) && *(*save + i) != '\n')
		i++;
	*line = ft_substr(*save, 0, i + 1);
	tmp = ft_substr(*save, i + 1, ft_strlen(*save) - i - 1);
	free(*save);
	*save = tmp;
	return (1);
}

int	readfile(int fd, char **save)
{
	char *buffer;
	int end;

	end = BUFFER_SIZE;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	*buffer = 0;
	while (end == BUFFER_SIZE && !findchar(buffer, '\n'))
	{
		if ((end = read(fd, buffer, BUFFER_SIZE)) < 0)
		{
			free(buffer);
			return (-1);
		}
		buffer[end] = 0;
		*save = ft_strjoin(*save, buffer);
	}
	free(buffer);
	if (end > 0)
		return (1);
	return (0);
}

char *get_next_line(int fd)
{
	static char *save = NULL;
	char *line;
	int end;

	line = NULL;
	end = 0;
	if (BUFFER_SIZE < 0 || fd < 0)
		return (NULL);
	if (save && findchar(save, '\n'))
	{
		firstline(&save, &line);
		return (line);
	}
	if ((end = readfile(fd, &save)) < 0)
	{
		line = malloc(1);
		*line = 0;
		return (line);
	}
	firstline(&save, &line);
	if (end == 0)
	{
		free(save);
		save = 0;
	}
	return (line);
}


