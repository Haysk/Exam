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
		return (NULL);
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
	if (start > ft_strlen(s))
		return (NULL);
	new = malloc(sizeof(char) * (len + 1));
	if (!s || !new)
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
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == c)
			return (i + 1);
		i++;
	}
	return (0);
}

int firstline(char **save, char **line)
{
	char *tmp;
	int i;

	i = 0;
	tmp = *save;
	if (*tmp == 0)
	{
		free(*save);
		*line = NULL;
		return (0);
	}
	i = findchar(tmp, '\n');
	if (i != 0)
	{
		*line = ft_substr(tmp, 0, i);
		tmp = ft_substr(tmp, i, ft_strlen(tmp));
		free(*save);
		*save = tmp;
	}
	else
	{
		*line = ft_substr(*save, 0, ft_strlen(tmp));
		free(*save);
		*save = NULL;
	}
	return (1);
}

int	readfile(int fd, char **save)
{
	char *buffer;
	int end;

	end = BUFFER_SIZE;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	*buffer = 0;
	while (end == BUFFER_SIZE && findchar(buffer, '\n') == 0)
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
	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	if (save && findchar(save, '\n') > 0)
	{
		firstline(&save, &line);
		return (line);
	}
	if ((end = readfile(fd, &save)) < 0)
	{
		return (NULL);
	}
	firstline(&save, &line);
	return (line);
}


