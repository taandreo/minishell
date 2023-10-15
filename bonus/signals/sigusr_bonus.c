#include "minishell_bonus.h"

void parent_sigusr_handler(int signum)
{
	(void)signum;
}

void child_sigusr_handler(int signum)
{
	if (signum == SIGUSR1)
		g_vars.state.status = 100;
}

void	trigger_child_sigusr(t_bool ignore)
{
	struct sigaction sa_ign;

	if (ignore)
		sa_ign.sa_handler = SIG_IGN;
	else
		sa_ign.sa_handler = child_sigusr_handler;
	sa_ign.sa_flags = 0;
	sigemptyset(&sa_ign.sa_mask);
	if (sigaction(SIGUSR1, &sa_ign, NULL) == -1) {
		perror("Error setting up parent signal handler");
		exit(EXIT_FAILURE);
	}
}

void	trigger_parent_sigusr(void)
{
	struct sigaction sa_parent;

	sa_parent.sa_handler = parent_sigusr_handler;
	sa_parent.sa_flags = 0;
	sigemptyset(&sa_parent.sa_mask);
	if (sigaction(SIGUSR1, &sa_parent, NULL) == -1) {
		perror("Error setting up parent signal handler");
		exit(EXIT_FAILURE);
	}
}