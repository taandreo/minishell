#include "tester.h"

void test_tokenizer(void **state)
{
	(void)state;
	char *input = ft_strdup("echo bla || echo ok (exit 5) && (echo $VAR2 abc$?*\"$VAR $?abc\"$VAR2 abc)");
	t_token_flags flags = init_token_flags(strlen(input));
	setup_malloc_behavior(SUCCESS_MALLOC,1);
	t_token_list *tokens = tokenizer(input, &flags);
	assert_null(tokens);
}

//void test_printf(void **state)
//{
//	(void)state;
//	setup_malloc_behavior(SUCCESS_MALLOC, 1);
//	int i = ft_dprintf(STDERR_FILENO, "BLA%i\n", 10);
//	assert_int_equal(i, 6);
//}
