/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigint.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:45:34 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/28 21:43:17 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_quit_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_vars.state.status = EXIT_OFFSET + SIGINT;
		g_vars.state.is_set = true;
		if (g_vars.heredoc_open)
			g_vars.close_heredoc = true;
		if (!g_vars.close_heredoc)
			free_mini_line(&g_vars);
		write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", false);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	heredoc_quit_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_vars.state.status = EXIT_OFFSET + SIGINT;
		g_vars.close_heredoc = true;
		write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", false);
		rl_on_new_line();
	}
}

void	start_signal_heredoc(void)
{
	struct sigaction	sa_heredoc_int;

	sa_heredoc_int.sa_handler = heredoc_quit_handler;
	sa_heredoc_int.sa_flags = 0;
	sigemptyset(&sa_heredoc_int.sa_mask);
	sigaction(SIGINT, &sa_heredoc_int, NULL);
}

void	start_signals_parent(void)
{
	struct sigaction	sa_parent_int;
	struct sigaction	sa_parent_quit;

	sa_parent_int.sa_handler = parent_quit_handler;
	sa_parent_int.sa_flags = 0;
	sigemptyset(&sa_parent_int.sa_mask);
	sigaction(SIGINT, &sa_parent_int, NULL);
	sa_parent_quit.sa_handler = SIG_IGN;
	sa_parent_quit.sa_flags = 0;
	sigemptyset(&sa_parent_quit.sa_mask);
	sigaction(SIGQUIT, &sa_parent_quit, NULL);
}

void	start_signal_forked(pid_t pid)
{
	struct sigaction	sa_forked;

	sa_forked.sa_flags = 0;
	sigemptyset(&sa_forked.sa_mask);
	if (pid == 0)
		sa_forked.sa_handler = SIG_DFL;
	else
		sa_forked.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_forked, NULL);
	sigaction(SIGQUIT, &sa_forked, NULL);
}
