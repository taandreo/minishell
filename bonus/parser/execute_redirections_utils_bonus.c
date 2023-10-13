#include "minishell_bonus.h"

void	input_file_to_stdin(int infile, t_vars *vars)
{
	if (!vars->changed_stdin)
	{
		vars->saved_stdin = dup(STDIN_FILENO);
		if (vars->saved_stdin == -1)
		{
			write(STDERR_FILENO, "minishell: dup2", ft_strlen("minishell: dup2"));
			perror("");
			vars->state.error = true;
			vars->state.status = GENERAL_ERROR;
			return ;
		}
		vars->changed_stdin = true;
	}
	if (dup2(infile, STDIN_FILENO) == -1)
	{
		write(STDERR_FILENO, "minishell: dup2", ft_strlen("minishell: dup2"));
		perror("");
		vars->state.error = true;
		vars->state.status = GENERAL_ERROR;
	}
}

void	output_file_to_stdout(int outfile, t_vars *vars)
{
	if (!vars->changed_stdout)
	{
		vars->saved_stdout = dup(STDIN_FILENO);
		if (vars->saved_stdin == -1)
		{
			write(STDERR_FILENO, "minishell: dup2",
					ft_strlen("minishell: dup2"));
			perror("");
			vars->state.error = true;
			vars->state.status = GENERAL_ERROR;
			return ;
		}
		vars->changed_stdout = true;
	}
	if (dup2(outfile, STDOUT_FILENO) == -1)
	{
		write(STDERR_FILENO, "minishell: dup2", ft_strlen("minishell: dup2"));
		perror("");
		vars->state.error = true;
		vars->state.status = GENERAL_ERROR;
	}
}

void	heredoc_to_stdin(int infile, t_vars *vars)
{
	close(infile);
	infile = open(".temp_file.txt", O_RDONLY);
	if (infile == -1)
	{
		write(STDERR_FILENO, "minishell: .temp_file.txt:",
				ft_strlen("minishell: .temp_file.txt:"));
		perror("");
		vars->state.error = true;
		vars->state.status = GENERAL_ERROR;
	}
	input_file_to_stdin(infile, vars);
	unlink(".temp_file.txt");
	close(infile);
}

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
