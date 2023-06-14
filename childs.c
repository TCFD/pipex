/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rciaze <rciaze@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 17:33:30 by rciaze            #+#    #+#             */
/*   Updated: 2023/02/03 15:50:09 by rciaze           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	f_child1(t_struct *my_struct, char **argv, char **envp)
{
	int	i;

	my_struct->file = open(argv[1], O_RDONLY);
	if (my_struct->file < 0)
	{
		i = -1;
		while (my_struct->paths[++i])
			free(my_struct->paths[i]);
		return (perror(argv[1]), free(my_struct->paths),
			free(my_struct), exit(EXIT_FAILURE));
	}
	my_struct->command = ft_split(argv[2], ' ');
	i = check_paths(my_struct);
	dup2(my_struct->file, STDIN_FILENO);
	dup2(my_struct->pipe_fd[1], STDOUT_FILENO);
	close (my_struct->pipe_fd[0]);
	exec(my_struct, envp, i);
	free_child(my_struct);
}

void	f_child2(t_struct *my_struct, char **argv, char **envp)
{
	int	i;

	my_struct->file = open(argv[4], O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (my_struct->file < 0)
	{
		i = -1;
		while (my_struct->paths[++i])
			free(my_struct->paths[i]);
		return (perror(argv[4]), free(my_struct->paths),
			free(my_struct), exit(EXIT_FAILURE));
	}	
	my_struct->command = ft_split(argv[3], ' ');
	i = check_paths(my_struct);
	dup2(my_struct->pipe_fd[0], STDIN_FILENO);
	dup2(my_struct->file, STDOUT_FILENO);
	close (my_struct->pipe_fd[1]);
	exec(my_struct, envp, i);
	free_child(my_struct);
}

void	free_child(t_struct *my_struct)
{
	int	i;

	i = -1;
	while (my_struct->command[++i])
		free(my_struct->command[i]);
	free(my_struct->command);
}

void	exec(t_struct *my_struct, char **envp, int i)
{
	char	*final_path;

	if (access(my_struct->command[0], F_OK) == 0
		&& access(my_struct->command[0], X_OK) == 0)
	{
		if (execve(my_struct->command[0], my_struct->command, envp))
			return (free(my_struct));
	}
	else
	{
		final_path = ft_strjoin(my_struct->paths[i], my_struct->command[0]);
		if (execve(final_path, my_struct->command, envp))
			return (free(final_path), free(my_struct));
	}
}

int	check_paths(t_struct *my_s)
{
	int		i;
	char	*final_path;

	if (access(my_s->command[0], F_OK) == 0
		&& access(my_s->command[0], X_OK) == 0)
		return (0);
	i = -1;
	while (my_s->paths[++i])
	{
		final_path = ft_strjoin(my_s->paths[i], my_s->command[0]);
		if (access(final_path, F_OK) == 0 && access(final_path, X_OK) == 0)
			return (free(final_path), i);
		free(final_path);
	}
	final_path = ft_substr(my_s->command[0], 0, ft_strlen(my_s->command[0]));
	if (access(final_path, F_OK) == 0 && access(final_path, X_OK) < 0
		&& !ft_strncmp(final_path, "./", 2))
		perror(final_path);
	else
		ft_printf("%s: command not found\n", my_s->command[0]);
	i = -1;
	while (my_s->paths[++i])
		free(my_s->paths[i]);
	return (free_child(my_s), free(final_path), free(my_s->paths),
		free(my_s), exit(EXIT_FAILURE), 0);
}
