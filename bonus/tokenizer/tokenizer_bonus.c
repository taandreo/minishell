#include "minishell_bonus.h"

t_token_list	*tokenizer(char *input, t_token_flags *flags)
{
	t_token_list	*tokens;
	char			*input_dup;
	size_t			position;
	char			c;

	position = 0;
	tokens = create_token_list();
	input_dup = ft_strdup(input);
	while (position < ft_strlen(input_dup))
	{
		c = input_dup[position];
		if (ft_is_space(c))
		{
			position++;
			continue ;
		}
		flags->status = tokenize_by_category(&input_dup, &position, &tokens, flags);
		if (flags->status != SUCCESS)
			break ;
	}
	if (flags->status == SUCCESS)
		add_token(&tokens, TOKEN_END, "");
	return (tokens);
}
