/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rciaze <rciaze@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 09:08:45 by rciaze            #+#    #+#             */
/*   Updated: 2023/02/03 19:00:56 by rciaze           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_struct	*my_struct;
	int			i;

	if (argc < 5)
		return (ft_printf("Trop peu d'arguments !\n"));
	if (argc > 5)
		return (ft_printf("Trop d'arguments !\n"));
	my_struct = ft_calloc(sizeof(t_struct), 1);
	if (!my_struct || pipe(my_struct->pipe_fd) != 0)
		return (0);
	my_struct->paths = get_path(envp);
	pipex(my_struct, argv, envp);
	i = -1;
	while (my_struct->paths[++i])
		free(my_struct->paths[i]);
	free(my_struct->paths);
	free(my_struct);
	return (0);
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
