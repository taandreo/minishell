#include "minishell_bonus.h"

void free_2_str_and_nullify(char **str1, char **str2)
{
	if (str1 && *str1)
		free_str_and_nullify(str1);
	if (str2 && *str2)
		free_str_and_nullify(str2);
}
