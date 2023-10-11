/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 21:38:26 by tairribe          #+#    #+#             */
/*   Updated: 2023/10/10 23:20:25 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell_bonus.h"

int	wait_process(t_pipeline *pipeline)
{
	pid_t	pid;
	int		status;

	while(pipeline)
	{
		pid = pipeline->cmd_part->pid;
		waitpid(pid, &status, 0);
	}
	return(check_for_sign(status));
}

static int	check_for_sign(int status)
{
	char	*description;

	if (WIFEXITED(status))
		return(WEXITSTATUS(status));
	else
	{
		if (WIFSIGNALED(status))
		{
			description = get_signal_description(WTERMSIG(status));
			ft_dprintf(STDERR_FILENO, "%s", description);
			if (WCOREDUMP(status))
				ft_dprintf(STDERR_FILENO, " (core dumped)");
			if (description != NULL)
				ft_dprintf(STDERR_FILENO, "\n");
			return(EXIT_OFFSET + WTERMSIG(status));
		}
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
	};

	return (descriptions[sig]);
}
