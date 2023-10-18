/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigusr_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebezerra <ebezerra@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:45:37 by ebezerra          #+#    #+#             */
/*   Updated: 2023/10/15 19:45:38 by ebezerra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_sigusr_handler(int signum)
{
	(void)signum;
}

void	child_sigusr_handler(int signum)
{
	if (signum == SIGUSR1)
		g_vars.state.kill_child = 1;
}

void	trigger_child_sigusr(t_bool ignore)
{
	struct sigaction	sa_ign;

	if (ignore == true)
		sa_ign.sa_handler = SIG_IGN;
	else
		sa_ign.sa_handler = child_sigusr_handler;
	sa_ign.sa_flags = 0;
	sigemptyset(&sa_ign.sa_mask);
	if (sigaction(SIGUSR1, &sa_ign, NULL) == -1)
	{
		perror("Error setting up parent signal handler");
		exit(EXIT_FAILURE);
	}
}

void	trigger_parent_sigusr(void)
{
	struct sigaction	sa_parent;

	sa_parent.sa_handler = parent_sigusr_handler;
	sa_parent.sa_flags = 0;
	sigemptyset(&sa_parent.sa_mask);
	if (sigaction(SIGUSR1, &sa_parent, NULL) == -1)
	{
		perror("Error setting up parent signal handler");
		exit(EXIT_FAILURE);
	}
}
