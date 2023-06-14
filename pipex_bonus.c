/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rciaze <rciaze@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 14:22:56 by rciaze            #+#    #+#             */
/*   Updated: 2023/02/03 15:29:56 by rciaze           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	pipex(t_struct *my_struct, char **argv, char **envp, int argc)
{
	if (!ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])))
		return (pipex_heredoc(my_struct, argv, envp, argc));
	my_struct->pid = ft_calloc(sizeof(int), argc - 3);
	my_struct->nb_of_forks = argc - 5;
	my_struct->pid[0] = fork();
	if (my_struct->pid[0] < 0)
		return (perror(NULL));
	if (my_struct->pid[0] == 0)
		f_child1(my_struct, argv, envp);
	pipex2(my_struct, argv, envp, argc);
}

void	pipex2(t_struct *my_struct, char **argv, char **envp, int argc)
{
	int	i;

	i = 1;
	while (i < my_struct->nb_of_forks + 1)
	{
		my_struct->pid[i] = fork();
		if (my_struct->pid[i] < 0)
			return (perror(NULL));
		if (my_struct->pid[i] == 0)
			nb_child(my_struct, argv, envp, i - 1);
		i++;
	}
	my_struct->pid[i] = fork();
	if (my_struct->pid[i] < 0)
		return (perror(NULL));
	if (my_struct->pid[i] == 0)
		last_child(my_struct, argv, envp, argc);
	close_all_pipes(my_struct);
	i = -1;
	while (++i < my_struct->nb_of_forks + 2)
		waitpid(my_struct->pid[i], NULL, 0);
	free(my_struct->pid);
}

void	pipex_heredoc(t_struct *my_struct, char **argv, char **envp, int argc)
{
	int	child1;
	int	child2;

	child1 = fork();
	if (child1 < 0)
		return (perror(NULL));
	if (child1 == 0)
		first_child_heredoc(my_struct, argv, envp);
	waitpid(child1, NULL, 0);
	child2 = fork();
	if (child2 < 0)
		return (perror(NULL));
	else if (!child2)
		last_child(my_struct, argv, envp, argc);
	close (my_struct->pipe_fd[0][0]);
	close (my_struct->pipe_fd[0][1]);
	waitpid(child2, NULL, 0);
	unlink("temp");
	return ;
}

void	last_child(t_struct *my_struct, char **argv, char **envp, int argc)
{
	int	i;

	if (!ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])))
		my_struct->file = open(argv[argc - 1], O_RDWR | O_CREAT
				| O_APPEND, 0666);
	else
		my_struct->file = open(argv[argc - 1], O_RDWR | O_CREAT
				| O_TRUNC, 0666);
	if (my_struct->file < 0)
	{
		i = -1;
		while (my_struct->paths[++i])
			free(my_struct->paths[i]);
		return (perror(argv[argc - 1]), free(my_struct->paths),
			free(my_struct), exit(EXIT_FAILURE));
	}
	my_struct->command = ft_split(argv[argc - 2], ' ');
	i = check_paths(my_struct);
	dup2(my_struct->pipe_fd[my_struct->nb_of_pipes - 1][0], STDIN_FILENO);
	dup2(my_struct->file, STDOUT_FILENO);
	close_all_pipes(my_struct);
	exec(my_struct, envp, i);
	free_child(my_struct);
}
