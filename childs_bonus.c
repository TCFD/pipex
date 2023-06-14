/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rciaze <rciaze@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 14:22:55 by rciaze            #+#    #+#             */
/*   Updated: 2023/02/03 15:32:16 by rciaze           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
	dup2(my_struct->pipe_fd[0][1], STDOUT_FILENO);
	close_all_pipes(my_struct);
	exec(my_struct, envp, i);
	free_child(my_struct);
}

void	first_child_heredoc(t_struct *my_struct, char **argv, char **envp)
{
	int		i;

	my_struct->file = open("temp", O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (my_struct->file < 0)
	{
		i = -1;
		while (my_struct->paths[++i])
			free(my_struct->paths[i]);
		return (perror(argv[1]), free(my_struct->paths),
			free(my_struct), exit(EXIT_FAILURE));
	}
	get_usr_input(my_struct, argv);
	if (close(my_struct->file))
		return (perror(argv[1]), exit(EXIT_FAILURE));
	my_struct->file = open("temp", O_RDONLY);
	if (my_struct->file < 0)
		return (perror(argv[1]), exit(EXIT_FAILURE));
	my_struct->command = ft_split(argv[3], ' ');
	i = check_paths(my_struct);
	dup2(my_struct->file, STDIN_FILENO);
	dup2(my_struct->pipe_fd[0][1], STDOUT_FILENO);
	close_all_pipes(my_struct);
	exec(my_struct, envp, i);
	free_child(my_struct);
}

void	get_usr_input(t_struct *my_struct, char **argv)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (ft_strncmp(line, argv[2], 1))
		{
			write(my_struct->file, line, ft_strlen(line));
			free (line);
			line = get_next_line(STDIN_FILENO);
		}
		else
			break ;
	}
	free (line);
}

void	nb_child(t_struct *my_struct, char **argv, char **envp, int i)
{
	int	j;

	my_struct->command = ft_split(argv[i + 3], ' ');
	j = check_paths(my_struct);
	dup2(my_struct->pipe_fd[i][0], STDIN_FILENO);
	dup2(my_struct->pipe_fd[i + 1][1], STDOUT_FILENO);
	close_all_pipes(my_struct);
	exec(my_struct, envp, j);
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
