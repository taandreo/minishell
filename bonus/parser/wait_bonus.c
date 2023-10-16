/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 21:38:26 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/16 00:24:54 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static char	*get_signal_description(int sig);
static int	check_for_sign(int status);

int	wait_process(t_pipeline *pipeline)
{
	pid_t	pid;
	int		status;

	while (pipeline)
	{
		pid = pipeline->cmd_part->pid;
		waitpid(pid, &status, 0);
		pipeline = pipeline->next;
	}
	return (check_for_sign(status));
}

static int	check_for_sign(int status)
{
	char	*description;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
	{
		if (WIFSIGNALED(status))
		{
			description = get_signal_description(WTERMSIG(status));
			write(STDERR_FILENO, description, ft_strlen(description));
			if (WCOREDUMP(status))
				write(STDERR_FILENO, " (core dumped)",
					ft_strlen(" (core dumped)"));
			if (description != NULL)
				write(STDERR_FILENO, "\n", ft_strlen("\n"));
			return (EXIT_OFFSET + WTERMSIG(status));
		}
		else
			return (EXIT_SUCCESS);
	}
}

static char	*get_signal_description(int sig)
{
	static char	*descriptions[EXIT_OFFSET] = {
	[SIGHUP] = "Hangup",
	[SIGQUIT] = "Quit",
	[SIGILL] = "Illegal instruction",
	[SIGTRAP] = "Trace/breakpoint trap",
	[SIGABRT] = "Aborted",
	[SIGFPE] = "Floating point exception",
	[SIGKILL] = "Killed",
	[SIGSEGV] = "Segmentation fault",
	[SIGALRM] = "Alarm clock",
	[SIGTERM] = "Terminated",
	[SIGUSR1] = "",
	[SIGINT] = "",
	};

	return (descriptions[sig]);
}
