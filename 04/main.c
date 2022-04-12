#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef struct s_list {
	char **args;
	int	pipe[2];
	struct	s_list *next;
	struct	s_list *prev;
} t_list;

int ft_strlen(char *str) {
	int i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i]) {
		i++;
	}
	return (i);
}

char *ft_strdup(char *str) {
	int i;
	char *cpy;

	i = 0;
	cpy = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i]) {
		cpy[i] = str[i];
		i++;
	}
	cpy[i] = 0;
	return (cpy);
}

char **create_args(char **av, int len) {
	char **args;
	int i;

	i = 0;
	args = malloc(sizeof(char*) * (len + 1));
	while (i < len) {
		args[i] = ft_strdup(av[i]);
		i++;
	}
	args[i] = NULL;
	return (args);
}

t_list *add_back(t_list *first, char **args) {
	t_list *node;
	t_list *tmp;

	node = malloc(sizeof(t_list));
	node->next = NULL;
	node->prev = NULL;
	node->args = args;
	if (!first)
		return (node);
	tmp = first;
	while(tmp) {
		if (!tmp->next) {
			tmp->next = node;
			node->prev = tmp;
			return (first);
		}
		tmp = tmp->next;
	}
	return first;
}

t_list	*free_list (t_list *first) {
	int i;
	t_list *tmp;
	while (first) {
		i = 0;
		while(first->args[i]) {
			free(first->args[i]);
			i++;
		}
		free(first->args);
		tmp = first;
		first = first->next;
		free(tmp);
	}
	return (NULL);
}

void	exec_cmd(t_list *first, char **env) {
	t_list *tmp;
	int	pid;

	if (!strcmp(first->args[0], "cd")) {
		if (first->args[1] && !first->args[2])
			chdir(first->args[1]);
		else
			printf("error\n");
	} else {
		tmp = first;
		while (tmp) {
			//if (tmp->next)
				pipe(tmp->pipe);
			pid = fork();
			if (pid == 0) {
				if (tmp->next && dup2(tmp->pipe[1], 1) < 0)
					printf("error1\n");
				if (tmp->prev && dup2(tmp->prev->pipe[0], 0) < 0)
					printf("error2\n");
				execve(tmp->args[0], tmp->args, env);
				printf("error3\n");
				exit(0);
			} else {
				waitpid(pid, NULL, 0);
				close(tmp->pipe[1]);
				if (tmp->prev)
					close(tmp->prev->pipe[0]);
				if (!tmp->next)
					close(tmp->pipe[0]);
			}
			tmp = tmp->next;
		}
	}
}

int	parse(char **av, char **env) {
	int i;
	int j;
	t_list *first;

	i = 0;
	first = NULL;
	while (av[i] && strcmp(av[i], ";"))
	{
		j = 0;
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
		{
			i++;
			j++;
		}
		first = add_back(first, create_args(av + (i - j), j));
		if (av[i] && strcmp(av[i], ";"))
			i++;
	}
	if (first) {
		exec_cmd(first, env);
		first = free_list(first);
	}
	return (i);
}

int main(int ac, char **av, char **env)
{
	int i;

	i = 0;
	if (ac < 2)
		return 0;
	av++;
	while (av[i])
	{
		i += parse(av + i, env);
		if (av[i])
			i++;
	}
    return 0;
}

