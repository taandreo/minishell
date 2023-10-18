/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 00:18:45 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/15 22:31:22 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_redirection_input(t_redirections *redir, t_vars *vars);
void	execute_redirection_output(t_redirections *redir, t_vars *vars);
void	execute_redirection_append(t_redirections *redir, t_vars *vars);

void	execute_redirections(t_command_part *data, t_vars *vars)
{
	t_redirections	*redir;

	if (data->redirections && data->redirections->redirection)
	{
		redir = data->redirections;
		while (redir)
		{
			if (redir->redirection->type == TOKEN_REDIRECTION_INPUT)
				execute_redirection_input(redir, vars);
			else if (redir->redirection->type == TOKEN_REDIRECTION_OUTPUT)
				execute_redirection_output(redir, vars);
			else if (redir->redirection->type == TOKEN_REDIRECTION_APPEND)
				execute_redirection_append(redir, vars);
			else if (redir->redirection->type == TOKEN_REDIRECTION_HEREDOC)
				heredoc_to_stdin(vars);
			if (vars->state.error == true)
				return ;
			redir = redir->next;
		}
	}
}

void	execute_redirection_input(t_redirections *redir, t_vars *vars)
{
	int				infile;
	t_redirection	*redirection;

	redirection = redir->redirection;
	infile = open(redirection->filename->value, O_RDONLY);
	if (infile == -1)
	{
		error_open_file(redirection->filename->value, vars);
		return ;
	}
	input_file_to_stdin(infile, vars);
	close(infile);
}

void	execute_redirection_output(t_redirections *redir, t_vars *vars)
{
	int				outfile;
	t_redirection	*redirection;

	redirection = redir->redirection;
	outfile = open(redirection->filename->value,
			O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (outfile == -1)
	{
		error_open_file(redirection->filename->value, vars);
		return ;
	}
	output_file_to_stdout(outfile, vars);
	close(outfile);
}

void	execute_redirection_append(t_redirections *redir, t_vars *vars)
{
	int				append_file;
	t_redirection	*redirection;

	redirection = redir->redirection;
	append_file = open(redirection->filename->value,
			O_CREAT | O_APPEND | O_WRONLY, 0666);
	if (append_file == -1)
	{
		error_open_file(redirection->filename->value, vars);
		return ;
	}
	output_file_to_stdout(append_file, vars);
	close(append_file);
}
