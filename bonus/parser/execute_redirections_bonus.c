#include "minishell_bonus.h"

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
		write(STDERR_FILENO, "minishell: ",
				ft_strlen("minishell: "));
		write(STDERR_FILENO, redirection->filename->value,
				ft_strlen(redirection->filename->value));
		write(STDERR_FILENO, ": ", ft_strlen(": "));
		perror("");
		vars->state.error = true;
		vars->state.is_set = true;
		vars->state.status = GENERAL_ERROR;
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
		write(STDERR_FILENO, "minishell: ", ft_strlen("minishell: "));
		write(STDERR_FILENO, redirection->filename->value,
				ft_strlen(redirection->filename->value));
		write(STDERR_FILENO, ": ", ft_strlen(": "));
		perror("");
		vars->state.error = true;
		vars->state.is_set = true;
		vars->state.status = GENERAL_ERROR;
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
		write(STDERR_FILENO, "minishell: ", ft_strlen("minishell: "));
		write(STDERR_FILENO, redirection->filename->value,
				ft_strlen(redirection->filename->value));
		write(STDERR_FILENO, ": ", ft_strlen(": "));
		perror("");
		vars->state.error = true;
		vars->state.is_set = true;
		vars->state.status = GENERAL_ERROR;
		return ;
	}
	output_file_to_stdout(append_file, vars);
	close(append_file);
}

void	execute_redirection_heredoc(t_redirections *redir)
{
	int		infile;
	char	*line;
	char	*trim_line;
	char	*limiter;

	infile = open_tmp_file();
	limiter = redir->redirection->filename->value;
	write(STDIN_FILENO, "> ", ft_strlen("> "));
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		trim_line = ft_strtrim(line, "\n");
		if (ft_strcmp(trim_line, limiter) == 0)
		{
			free(line);
			free(trim_line);
			break ;
		}
		write(infile, line, ft_strlen(line));
		free(line);
		free(trim_line);
		write(STDIN_FILENO, "> ", ft_strlen("> "));
		line = get_next_line(STDIN_FILENO);
	}
	close(infile);
}