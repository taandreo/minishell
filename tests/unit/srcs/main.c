#include "tester.h"

int	main(void)
{
	int total_failures = 0;

	const struct CMUnitTest test_add_token[] = {
			cmocka_unit_test(test_create_token_list_success),
			cmocka_unit_test(test_create_token_list_fail),
			cmocka_unit_test(test_add_token_first_malloc_fail),
			cmocka_unit_test(test_add_token_second_malloc_fail),

	};
	total_failures += cmocka_run_group_tests(test_add_token, NULL, NULL);

	return(total_failures);
}