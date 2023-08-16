#include "minishell_bonus.h"
//
//t_bool	free_vars_and_return_true(char **dir, char *cmd, t_grammar *grammar);
//t_bool	check_path_var(char *token, t_grammar *grammar);
//void	free_dir(char **dir);
//
t_bool	is_builtin(char *token)
{
	if (ft_strcmp(token, "echo") == 0
		|| ft_strcmp(token, "cd") == 0
		|| ft_strcmp(token, "pwd") == 0
		|| ft_strcmp(token, "export") == 0
		|| ft_strcmp(token, "unset") == 0
		|| ft_strcmp(token, "env") == 0
		|| ft_strcmp(token, "exit") == 0)
		return (true);
	return (false);
}
//
//t_bool	is_command(char *token)
//{
//	struct stat	stats;
//
//	if (stat(token, &stats) == -1)
//	{
//		ft_dprintf(2, "Failed to get file status\n");
//		exit (GENERAL_ERROR);
//	}
//	if (S_ISDIR(stats.st_mode))
//	{
//		ft_dprintf(2, "minishell: %s: Is a directory\n", token);
//		exit(NOTEXEC);
//	}
//	if (access(token, F_OK) == 0 && access(token, X_OK) == -1)
//	{
//		ft_dprintf(2, "minishell: %s: Permission denied\n", token);
//		exit(NOTEXEC);
//	}
//	if (access(token, F_OK) == -1)
//		return (check_path_var(token, grammar));
//	grammar->has_command = true;
//	return (true);
//}
//
//t_bool	check_path_var(char *token)
//{
//	char	*path;
//	char	**dir;
//	char	*tmp;
//	char	*cmd;
//	int		i;
//
//	i = 0;
//	path = getenv("PATH");
//	dir = ft_split(path, ':');
//	while (dir[i])
//	{
//		tmp = ft_strjoin(dir[i], "/");
//		cmd = ft_strjoin(tmp, token);
//		free(tmp);
//		if (access(cmd, X_OK) == 0)
//			free_vars_and_return_true(dir, cmd, grammar);
//		free(cmd);
//		cmd = NULL;
//		i++;
//	}
//	free_dir(dir);
//	return (false);
//}
//
//void	free_dir(char **dir)
//{
//	int	i;
//
//	i = 0;
//	while (dir[i])
//		free(dir[i++]);
//	free(dir);
//}
//
//t_bool	free_vars_and_return_true(char **dir, char *cmd)
//{
//	free(cmd);
//	free_dir(dir);
//	grammar->has_command = true;
//	return (true);
//}
