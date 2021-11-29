#include <unistd.h>

int	doubl(char *str, int i)
{
	int j;
	
	j = 0;
	while (j < i)
	{
		if (str[i] == str[j])
			return (1);
		j++;
	}
	return (0);
}

int	findchar(char c, char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (c == str[i])
			return (1);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	int	i;

	i = 0;
	if (ac == 3)
	{
		while (av[1][i])
		{
			if (!doubl(av[1], i) && findchar(av[1][i], av[2]))
				write(1,av[1] + i,1);
			i++;
		}
	}
	write (1, "\n", 1);
	return (0);
}
