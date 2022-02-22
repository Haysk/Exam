#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct s_draw
{
	int	width;
	int	height;
	char	bg;
	char	**tab;
}	t_draw;

typedef struct s_cercle
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	c;

}	t_cercle;

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
		i++;
	return (i);
}

void ft_putstr(char *str)
{
	if (!str || !str[0])
		return ;
	write(1, str, ft_strlen(str));
}

void	put_draw(t_draw *draw)
{
	int i;
	int j;

	i = 0;
	while (i < draw->height)
	{
		j = 0;
		while(j < draw->width)
		{
			write(1, &draw->tab[i][j], 1);
			j++;
		}
		write(1,"\n",1);
		i++;
	}
}

int	get_bg(FILE *file, t_draw *draw)
{
	if (fscanf(file, "%d %d %c\n", &draw->width, &draw->height, &draw->bg) != 3)
		return (1);
	if (draw->width < 0 || draw->width > 300 || draw->height < 0 || draw->height > 300)
		return (1);
	return (0);
}

void	create_bg(t_draw *draw)
{
	int	i;
	int	j;

	i = 0;
	draw->tab = malloc(sizeof(char*) * draw->height);
	while (i < draw->height)
	{
		draw->tab[i] = malloc(sizeof(char) * draw->width);
		j = 0;
		while (j < draw->width)
		{
			draw->tab[i][j] = draw->bg;
//			printf("%c", draw->tab[i][j]);
			j++;
		}
//		printf("\n");
		i++;
	}
}

float	is_cercle(int x, int y, t_cercle cercle)
{
	float dist;

	dist = sqrtf(powf(x - cercle.x, 2) + powf(y - cercle.y, 2));
		if (dist <= cercle.radius)
		{
			if ((cercle.radius - dist) < (float)1)
				return (2);
			return (1);
		}
	return (0);
}

void	draw_cercle(t_draw *draw, t_cercle cercle)
{
	int	i;
	int	j;

	i = 0;
	while(i < draw->height)
	{
		j = 0;
		while(j < draw->width)
		{
			if (cercle.type == 'C' &&  is_cercle(i, j, cercle) != 0)
				draw->tab[i][j] = cercle.c;
			else if (cercle.type == 'c' && is_cercle(i, j, cercle) == 2)
				draw->tab[i][j] = cercle.c;
			j++;
		}
		i++;
	}
}
int	put_cercle(FILE *file, t_draw *draw)
{
	t_cercle	cercle;
	int	ret;

	while ((ret = fscanf(file, "%c %f %f %f %c\n", &cercle.type, &cercle.x, &cercle.y, &cercle.radius, &cercle.c)) == 5)
	{
		if (cercle.radius <= (float)0 || (cercle.type != 'c' && cercle.type != 'C'))
			return (1);
		draw_cercle(draw, cercle);
	}
	if (ret == -1)
		return (0);
	return (1);

}

int	main(int ac, char **av)
{
	FILE *file;
	t_draw	draw;

	if (ac != 2)
	{	
		ft_putstr("Error: argument\n");
		return (1);
	}
	file = fopen(av[1], "r");
	if (!file || get_bg(file, &draw))
	{	
		ft_putstr("Error: Operation file corrupted\n");
		return (1);
	}
	create_bg(&draw);
	if (put_cercle(file, &draw))
	{
		ft_putstr("Error: Operation file corrupted\n");
		return (1);
	}
	put_draw(&draw);
	return (0);

}
