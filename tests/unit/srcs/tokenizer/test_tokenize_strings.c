#include "tester.h"

void test_tokenize_strings_fail(void **state)
{
	(void)state;
	char *str = strdup("abc\"$VAR2 $? anc\"cde");
	t_token_flags flags = init_token_flags(strlen(str));
	t_token_list *tokens = create_token_list();
	size_t pos = 0;
	setup_malloc_behavior(39, 1);
	size_t status = tokenize_strings(&str, &pos, &tokens, &flags);
	assert_int_equal(status, 2);
	assert_null(tokens);
}
