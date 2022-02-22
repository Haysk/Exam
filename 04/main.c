#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

typedef struct s_list {
	char **args;
	int	type;
	int pipe[2];
	struct s_list *next; 
	struct s_list *prev; 
} t_list;

int	ft_strlen(char *str) {
	int i;

	i = 0;
	if (!str || !str[0])
		return 0;
	while (str[i])
		i++;
	return i;
}

char *ft_strdup(char *str) {
	char *cp;
	int i;

	cp = malloc(sizeof(char) * (ft_strlen(str) + 1));
	i = 0;
	while (str[i])
	{
		cp[i] = str[i];
		i++;
	}
	cp[i] = 0;
	return cp;
}

void	free_list(t_list *first) {
	t_list *tmp;
	int i;
	
	if (!first)
		return;
	while (first)
	{
		i = 0;
		while (first->args[i])
		{
			free(first->args[i]);
			i++;
		}
		free(first->args);
		tmp = first;
		first = first->next;
		free(tmp);
	}
	first = NULL;
}

char**	create_cmd(char **av, int len) {
	int i;
	char **args;

	i = 0;
	args = malloc(sizeof(char*) * (len + 1));
	while (i < len)
	{
		args[i] = ft_strdup(av[i]);
		i++;
	}
	args[i] = 0;
	return (args);
}

t_list*	add_list(t_list *first, char **args) {
	t_list *node;
	t_list *tmp;

	node = malloc(sizeof(t_list));
	node->args = args;
	node->next = NULL;
	node->prev = NULL;
	node->type = 0;
	if (args == NULL)
		node->type = 1;
	if (!first)
	{
		first = node;
		return first;
	}
	tmp = first;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	node->prev = tmp;
	return first;
}

void	cd(t_list *cmd) {
	int i;

	i = 0;
	while (cmd->args[i])
		i++;
	if (i != 2)
		printf("error cd");
	else
		chdir(cmd->args[1]);
}

void	exec_cmd(char **env, t_list *cmd) {
	(void)env;
	execve(cmd->args[0], cmd->args, env);
	printf("error: cannot execute %s\n", cmd->args[0]);
	exit(0);
}

void	exec_cmds(char **env, t_list *first)
{
	t_list	*tmp;
	int		pid;

	tmp = first;
	while (tmp)
	{
			pipe(tmp->pipe);
		if (strcmp(tmp->args[0], "cd") == 0)
			cd(tmp);
		else {
			pid = fork();
			if (pid == 0) {
				if (tmp->next && dup2(tmp->pipe[1], 1) < 0)
					printf("error");
				if (tmp->prev && dup2(tmp->prev->pipe[0], 0) < 0)
					printf("error");
				exec_cmd(env, tmp);
			} else {
				waitpid(pid, NULL, 0);
				close(tmp->pipe[1]);
				if (!tmp->next)
					close(tmp->pipe[0]);
				if (tmp->prev)
					close(tmp->prev->pipe[0]);
			}
		}
		tmp = tmp->next;
	}
}

int	parse(char **av, char **env, t_list **first)
{
	int i;
	int	j;
	char **args;

	i = 0;
	args = NULL;
	while (av[i] && strcmp(av[i], ";") != 0)
	{
		j = 0;
		while (av[i] && strcmp(av[i], "|") != 0 && strcmp(av[i], ";") != 0)
		{
			i++;
			j++;
		}
		args = create_cmd(av + (i - j), j);
		*first = add_list(*first, args);
		if (av[i] && strcmp(av[i], "|") == 0)
			i++;
	}
	exec_cmds(env, *first);
	free_list(*first);
	*first = NULL;
	return (i);
}

int main(int ac, char **av, char **env)
{
	int i;
	t_list *first;

	i = 0;
	first = NULL;
	(void)env;
	if (ac < 2)
		return (0);
	av++;
	while (av[i])
	{
		i += parse(av + i, env, &first);
		if (av[i])
		   i++;
	}
    return 0;
}

