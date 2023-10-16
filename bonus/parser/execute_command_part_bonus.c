/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_part_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 01:39:08 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/16 01:58:39 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	execute_builtin_command(t_command_part *data, char **args, t_vars *vars)
{
	args++;
	if (data->type == TOKEN_PWD)
		return (builtin_pwd(args));
	if (data->type == TOKEN_CD)
		return (builtin_cd(args));
	if (data->type == TOKEN_ECHO)
		return (builtin_echo(args));
	if (data->type == TOKEN_ENV)
		return (builtin_env(args));
	if (data->type == TOKEN_EXPORT)
		return (builtin_export(args));
	if (data->type == TOKEN_UNSET)
		return (builtin_unset(args));
	if (data->type == TOKEN_EXIT)
		return (builtin_exit(args, vars));
	return (0);
}

void	execute_builtin(t_command_part *data, t_vars *vars)
{
	if (data->forked)
	{
		vars->state.status = execute_builtin_command(data, data->args, vars);
		free_minishell(vars);
		exit(vars->state.status);
	}
	else if (is_builtin_token(data->type) && !data->forked)
	{
		vars->state.status = execute_builtin_command(data, data->args, vars);
		if (vars->changed_stdin)
		{
			restore_stdin(vars->saved_stdin, vars);
			vars->changed_stdin = false;
		}
		if (vars->changed_stdout)
		{
			restore_stdout(vars->saved_stdout, vars);
			vars->changed_stdout = false;
		}
	}
}

void	handle_exec_errors(t_vars *vars, char **envp)
{
	perror("minishell: ");
	free_minishell(vars);
	if (envp)
	{
		ft_freemt((void **)envp);
		envp = NULL;
	}
	vars->state.status = EXIT_FAILURE;
	exit(vars->state.status);
}

void	exec_external_command(t_command_part *data, t_vars *vars)
{
	char	**envp;

	data->cmd_path = cmd_path_routine(data->u_cmd.cmd_name->value,
			data->type, vars);
	if (!data->cmd_path)
	{
		free_minishell(vars);
		exit(vars->state.status);
	}
	envp = list_to_envp();
	execve(data->cmd_path, data->args, envp);
	handle_exec_errors(vars, envp);
}

int	execute_command_part(t_command_part *data, t_vars *vars)
{
	char			**envp;

	if (update_cmd_part_values(data, vars) != SUCCESS)
		return (vars->state.status);
	data->args = list_to_args(data->arguments, data);
	execute_redirections(data, vars);
	if (vars->state.error == true)
	{
		if (data->forked == true)
		{
			free_minishell(vars);
			exit(vars->state.status);
		}
		return (vars->state.status);
	}
	if (is_token_cmd_name(data->type))
		exec_external_command(data, vars);
	else if (data->type == TOKEN_GROUP)
		execute_command(&data->u_cmd.grouping->enclosed_cmd, vars);
	else
		execute_builtin(data, vars);
	return (vars->state.status);
}
