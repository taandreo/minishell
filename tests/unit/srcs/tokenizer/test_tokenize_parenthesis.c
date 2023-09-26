#include "tester.h"

void test_tokenize_parenthesis_fail(void **state)
{
	(void)state;
	char *input = ")";
	size_t pos = 0;
	t_token_list *tokens = create_token_list();
	t_token_flags flags = init_token_flags(strlen(input));
	setup_malloc_behavior(0, 1);
	int status = tokenize_parenthesis(input, &pos, &tokens, &flags);
	assert_int_equal(status, 2);
	assert_null(tokens);
}