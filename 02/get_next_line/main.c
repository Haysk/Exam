#include "get_next_line.h"

int main(int ac, char **av)
{
	int fd;
	int i;
	char *line;

	line = 0;
	fd = 0;
	i = 1;
	if (ac == 2)
		fd = open(av[1], O_RDONLY);
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("%i : %s", i, line);
		free(line);
		line = get_next_line(fd);
		i++;
	}
	return (0);
}
