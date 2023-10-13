/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 19:45:42 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/12 19:56:05 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int	output_redirection(t_redirections *files)
{
	int		fd;
	char	*file;

	fd = -2;
	while(files)
	{
		if (files->redirection->type == TOKEN_REDIRECTION_OUTPUT
			|| files->redirection->type == TOKEN_REDIRECTION_APPEND)
		{
			if (fd != -2)
				close(fd);
			file = files->redirection->filename->value;
			if (files->redirection->type == TOKEN_REDIRECTION_OUTPUT)
				fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if (files->redirection->type == TOKEN_REDIRECTION_APPEND)
				fd = open(file, O_WRONLY | O_APPEND, 0666);
			if (fd == -1)
			{
				print_perror(NULL, file);
				return (-1);
			}
		}
	}
	return (fd);
}

int	set_redirections(t_redirections *files)
{
	int	out_fd;

	out_fd = output_redirection(files);
	printf("outfd: %i", out_fd);
	if (out_fd == -1)
		return (-1);
	if (out_fd != -2)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	return (0);
}