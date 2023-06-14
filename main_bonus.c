/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rciaze <rciaze@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 14:22:53 by rciaze            #+#    #+#             */
/*   Updated: 2023/02/03 15:54:00 by rciaze           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	main(int ac, char **av, char **envp)
{
	t_struct	*my_struct;
	int			i;

	if (ac < 5 || (!ft_strncmp(av[1], "here_doc", ft_strlen(av[1])) && ac > 6))
		return (ft_printf("Mauvaise utilisation.\n"));
	my_struct = ft_calloc(sizeof(t_struct), 1);
	if (!my_struct)
		return (0);
	my_struct->nb_of_pipes = ac - 4;
	if (!ft_strncmp(av[1], "here_doc", ft_strlen(av[1])))
		my_struct->nb_of_pipes = 1;
	my_struct->pipe_fd = malloc(sizeof(int [2]) * my_struct->nb_of_pipes);
	if (!my_struct->pipe_fd)
		return (free(my_struct), 0);
	malloc_and_pipe(my_struct, -1);
	my_struct->paths = get_path(envp);
	pipex(my_struct, av, envp, ac);
	i = -1;
	while (my_struct->paths[++i])
		free(my_struct->paths[i]);
	i = -1;
	while (++i < my_struct->nb_of_pipes)
		free(my_struct->pipe_fd[i]);
	return (free(my_struct->paths), free(my_struct->pipe_fd),
		free(my_struct), 0);
}

void	malloc_and_pipe(t_struct *my_struct, int i)
{
	while (++i < my_struct->nb_of_pipes)
	{
		my_struct->pipe_fd[i] = ft_calloc(sizeof(int), 2);
		if (my_struct->pipe_fd[i] == NULL)
		{
			i++;
			while (--i > -1)
				free(my_struct->pipe_fd[i]);
			free(my_struct->pipe_fd);
			free(my_struct);
			exit(EXIT_FAILURE);
		}
		else if (pipe(my_struct->pipe_fd[i]) < 0)
		{
			i++;
			while (--i > -1)
				free(my_struct->pipe_fd[i]);
			free(my_struct->pipe_fd);
			free(my_struct);
			exit(EXIT_FAILURE);
		}
	}
}

char	**get_path(char **envp)
{
	char	**paths;
	char	*temp_path;
	int		i;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = -1;
	while (paths[++i])
	{
		temp_path = ft_strjoin(paths[i], "/");
		free(paths[i]);
		paths[i] = ft_strdup(temp_path);
		free(temp_path);
	}
	return (paths);
}
