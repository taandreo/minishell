#include "tester.h"

int main(void)
{
	int total_failures = 0;

	const struct CMUnitTest test_add_token[] = {
			cmocka_unit_test(test_create_token_list_success),
			cmocka_unit_test(test_create_token_list_fail),
			cmocka_unit_test(test_add_token_first_malloc_fail),
			cmocka_unit_test(test_add_token_second_malloc_fail),
			cmocka_unit_test(test_add_token_success),
	};

	const struct CMUnitTest test_tokenizer_helper_functions[] = {
			cmocka_unit_test(test_peek_next),
			cmocka_unit_test(test_is_string_start),
			cmocka_unit_test(test_has_quotes),
			cmocka_unit_test(test_initialize_var_string),
	};

	const struct CMUnitTest test_tokenizer_functions[] = {
			cmocka_unit_test(test_tokenize_operators_success),
			cmocka_unit_test(test_tokenize_redirections),
	};

	total_failures += cmocka_run_group_tests(test_add_token, NULL, NULL);
	total_failures += cmocka_run_group_tests(test_tokenizer_helper_functions,
			NULL, NULL);
	total_failures += cmocka_run_group_tests(test_tokenizer_functions, NULL, NULL);

	return (total_failures);
}
