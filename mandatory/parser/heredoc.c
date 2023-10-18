/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 01:11:24 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/15 22:28:32 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_tmp_file(void)
{
	int	infile;

	infile = open(".temp_file.txt", O_WRONLY | O_CREAT | O_APPEND,
			0666);
	if (infile == -1)
	{
		write(STDERR_FILENO, "minishell: .temp_file.txt: ",
			ft_strlen("minishell: .temp_file.txt: "));
		perror("");
	}
	return (infile);
}

int	write_heredoc_line(int infile, char *line, char *limiter, t_vars *vars)
{
	char	*trim_line;

	if (vars->close_heredoc)
	{
		free(line);
		return (1);
	}
	trim_line = ft_strtrim(line, "\n");
	if (ft_strcmp(trim_line, limiter) == 0)
	{
		free(line);
		free(trim_line);
		return (1);
	}
	write(infile, line, ft_strlen(line));
	free(trim_line);
	write(STDIN_FILENO, "> ", ft_strlen("> "));
	return (0);
}

void	execute_redirection_heredoc(t_redirections *redir, t_vars *vars)
{
	int		infile;
	char	*line;
	char	*limiter;

	infile = open_tmp_file();
	limiter = redir->redirection->filename->value;
	write(STDIN_FILENO, "> ", ft_strlen("> "));
	line = get_next_line(STDIN_FILENO);
	if (vars->close_heredoc)
	{
		free(line);
		line = NULL;
	}
	while (line != NULL)
	{
		if (write_heredoc_line(infile, line, limiter, vars) == 1)
			break ;
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	close(infile);
}

void	heredoc_to_stdin(t_vars *vars)
{
	int	infile;

	infile = open(".temp_file.txt", O_RDONLY);
	if (infile == -1)
	{
		write(STDERR_FILENO, "minishell: .temp_file.txt:",
			ft_strlen("minishell: .temp_file.txt:"));
		perror("");
		vars->state.error = true;
		vars->state.is_set = true;
		vars->state.status = GENERAL_ERROR;
	}
	input_file_to_stdin(infile, vars);
	unlink(".temp_file.txt");
	close(infile);
}

void	open_heredoc(t_redirections *redirections, t_vars *vars)
{
	t_redirections	*current;

	current = redirections;
	start_signal_heredoc();
	while (current)
	{
		if (current->redirection->type == TOKEN_REDIRECTION_HEREDOC)
			execute_redirection_heredoc(current, vars);
		if (vars->close_heredoc)
			break ;
		current = current->next;
	}
}
