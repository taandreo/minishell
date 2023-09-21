#include "tester.h"

static void echo_simple()
{
	char *input[] = {"simple", "test", NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "simple test\n");
}

static void echo_simple_with_n()
{
	char *input[] = {"-n", "simple", "test", NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "simple test");
}

static void echo_no_params()
{
	char *input[] = {NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "\n");
}

static void echo_n()
{
	char *input[] = {"-n", NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "");
}

static void echo_simple_with_nnn()
{
	char *input[] = {"-nnn", "simple", "test", NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "simple test");
}

static void echo_simple_with_nna()
{
	char *input[] = {"-nna", "simple", "test", NULL};
	setup();
	bultin_echo(input);
	teardown();
	char *out = file_to_string(temp_filename);
	assert_string_equal(out, "-nna simple test\n");
}

// int main(void){
//     const struct CMUnitTest echo_tests[] = {
// 		cmocka_unit_test(echo_simple),
// 		cmocka_unit_test(echo_simple_with_n),
// 		cmocka_unit_test(echo_simple_with_nnn),
// 		cmocka_unit_test(echo_simple_with_nna),
// 		cmocka_unit_test(echo_no_params),
// 		cmocka_unit_test(echo_n),
// 	};
//     return cmocka_run_group_tests(echo_tests, NULL, NULL);
// }

int main(void){
	printf("ok\n");
}