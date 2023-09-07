#include "tester.h"

//void test_tokenize_quotes(void **state)
//{
//	(void)state;
//}

void test_initialize_var_string(void **state)
{
	(void)state;
//	t_token_list *tokens = create_token_list();
	char *input = "\"echo \"abc";
	size_t len = strlen(input);
	t_token_flags flags = init_flags(len);
	const char *start = input + 0;
	flags.handle_quotes = true;
	flags.quote_type = '\"';
	if (!flags.string)
		flags.string = ft_strdup("");
	flags.var = initialize_var_string(input, 0, &flags, start);
	assert_string_equal(flags.var, "\"echo \"abc");
}