#include "minishell_bonus.h"

char *builtin_type_to_value(t_token_type  type)
{
	if (type == TOKEN_ECHO)
		return ("echo");
	if (type == TOKEN_CD)
		return ("cd");
	if (type == TOKEN_PWD)
		return ("pwd");
	if (type == TOKEN_EXPORT)
		return ("export");
	if (type == TOKEN_UNSET)
		return ("unset");
	if (type == TOKEN_ENV)
		return ("env");
	if (type == TOKEN_EXIT)
		return ("exit");
	return (NULL);
}

t_string	*delete_first_node(t_string *str)
{
	t_string	*next;

	next = str->next;
	free(str->value);
	free(str);
	return (next);
}

void	*general_error_ambiguous_redirect(t_vars *vars)
{
	write(STDERR_FILENO, "minishell: ambiguous redirect\n",
			ft_strlen("minishell: ambiguous redirect\n"));
	vars->state.status = GENERAL_ERROR;
	return (NULL);
}
