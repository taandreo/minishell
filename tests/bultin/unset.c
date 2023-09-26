#include "tester.h"

void	unset_no_args()
{
	char *empty_input[]  = {NULL};

	setup();
	bultin_unset(empty_input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	assert_string_equal(out, "");
}

void	unset_1_args()
{
	char *export_input[] = {"BANANA=banana", NULL};
	char *unset_input[] = {"BANANA", NULL};
	char *empty_input[]  = {NULL};

	bultin_export(export_input);
	assert_true(search_env("BANANA") != NULL);
	bultin_unset(unset_input);
	assert_true(search_env("BANANA") == NULL);
}

void	unset_2_args()
{
	char *export_input[] = {"BANANA=banana", "LIMAO=limao", NULL};
	char *unset_input[] = {"BANANA", "LIMAO", NULL};
	char *empty_input[]  = {NULL};

	bultin_export(export_input);
	assert_true(search_env("BANANA") != NULL && search_env("LIMAO") != NULL);
	bultin_unset(unset_input);
	assert_true(search_env("BANANA") == NULL && search_env("LIMAO") == NULL);
}

void	unset_2_args_no_value()
{
	char *export_input[] = {"BANANA", "LIMAO", NULL};
	char *unset_input[] = {"BANANA", "LIMAO", NULL};
	char *empty_input[] = {NULL};

	bultin_export(export_input);
	assert_true(search_env("BANANA") != NULL && search_env("LIMAO") != NULL);
	bultin_unset(unset_input);
	assert_true(search_env("BANANA") == NULL && search_env("LIMAO") == NULL);
}

void	unset_2_args_with_equal()
{
	char *export_input[] = {"BANANA=banana", "LIMAO=limao", NULL};
	char *unset_input[]  = {"BANANA=banana", "LIMAO=limao", NULL};
	char *empty_input[]  = {NULL};

	bultin_export(export_input);
	assert_true(search_env("BANANA") != NULL && search_env("LIMAO") != NULL);
	bultin_unset(unset_input);
	assert_true(search_env("BANANA") == NULL && search_env("LIMAO") == NULL);
}

void	unset_empty_value()
{
	char *input[]       = {"limao", NULL};
	char *empty_input[] = {NULL};

	char *limao = NULL;
	setup();
	bultin_unset(input);
	bultin_unset(empty_input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	limao = strstr(out, "declare -x limao");
	assert_true(limao != NULL);	
}

void	unset_empty_2_value()
{
	char *input[]       = {"limao", "abacaxi", NULL};
	char *empty_input[] = {NULL};

	char *limao = NULL;
	char *abacaxi = NULL;
	setup();
	bultin_unset(input);
	bultin_unset(empty_input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	limao = strstr(out, "declare -x limao");
	abacaxi = strstr(out, "declare -x abacaxi");
	assert_true(limao != NULL && abacaxi != NULL);	
}

void	unset_3_values()
{
	char *input[]       = {"limao", "abacaxi", "fruta=morango", NULL};
	char *empty_input[] = {NULL};

	char *limao = NULL;
	char *abacaxi = NULL;
	char *morango = NULL;
	setup();
	bultin_unset(input);
	bultin_unset(empty_input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	limao =   strstr(out, "declare -x limao");
	abacaxi = strstr(out, "declare -x abacaxi");
	morango = strstr(out, "declare -x fruta=\"morango\"");
	assert_true(limao != NULL && abacaxi != NULL && morango != NULL);
}

void	unset_error_1teste()
{
	char *input[]       = {"1teste", NULL};
	char *empty_input[] = {NULL};

	char *teste = NULL;
	setup();
	bultin_unset(input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	assert_string_equal(out, "minishell: unset: `1teste': not a valid identifier\n");
}

void	unset_error_ok()
{
	char *input[]       = {"ok-", NULL};
	char *empty_input[] = {NULL};

	char *teste = NULL;
	setup();
	bultin_unset(input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	assert_string_equal(out, "minishell: unset: `ok-': not a valid identifier\n");
}

void	unset_error_equal()
{
	char *input[]       = {"=teste", NULL};
	char *empty_input[] = {NULL};

	char *teste = NULL;
	setup();
	bultin_unset(input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	assert_string_equal(out, "minishell: unset: `=teste': not a valid identifier\n");
}

void	unset_2_errors()
{
	char *input[]       = {"=", "=teste", NULL};
	char *empty_input[] = {NULL};

	char *teste = NULL;
	setup();
	bultin_unset(input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	assert_string_equal(out, "minishell: unset: `=': not a valid identifier\nminishell: unset: `=teste': not a valid identifier\n");
}

void	unset_two_equals()
{
	char *input[]       = {"limao=limao=limao", NULL};
	char *empty_input[] = {NULL};

	char *limao = NULL;
	setup();
	bultin_unset(input);
	bultin_unset(empty_input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	limao = strstr(out, "declare -x limao=\"limao=limao\"");
	assert_true(limao != NULL);	
}

void	unset_2_args_one_error()
{
	char *input[]       = {"1_b", "teste=teste", NULL};
	char *empty_input[] = {NULL};

	char *teste = NULL;
	setup();
	bultin_unset(input);
	teardown();
	char *out  = file_to_string(TEMP_FILENAME);
	setup();
	bultin_unset(empty_input);
	teardown();
	char *out2 = file_to_string(TEMP_FILENAME);
	teste = strstr(out2,  "declare -x teste=\"teste\"");
	assert_string_equal(out, "minishell: unset: `1_b': not a valid identifier\n");
	assert_true(teste != NULL);
	// free(out);
	// free(out2);
}

int	run_unset_tests(void)
{
	const struct CMUnitTest unset_tests[] = {
		cmocka_unit_test(unset_no_args),
		cmocka_unit_test(unset_1_args),
		cmocka_unit_test(unset_2_args),
		cmocka_unit_test(unset_2_args_no_value),
	};
    return cmocka_run_group_tests(unset_tests, NULL, NULL);
}