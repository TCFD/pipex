/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rciaze <rciaze@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 17:12:48 by rciaze            #+#    #+#             */
/*   Updated: 2023/02/02 10:31:20 by rciaze           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	pipex(t_struct *my_struct, char **argv, char **envp)
{
	int	child1;
	int	child2;

	child1 = fork();
	if (child1 < 0)
		return (perror(NULL));
	else if (child1 == 0)
		f_child1(my_struct, argv, envp);
	child2 = fork();
	if (child2 < 0)
		return (perror(NULL));
	else if (!child2)
		f_child2(my_struct, argv, envp);
	close (my_struct->pipe_fd[0]);
	close (my_struct->pipe_fd[1]);
	waitpid(child1, NULL, 0);
	waitpid(child2, NULL, 0);
	return ;
}
