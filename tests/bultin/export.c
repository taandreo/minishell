#include "tester.h"

void	simple_export_1_term()
{
	char *input[]       = {"BANANA=banana", NULL};
	char *empty_input[] = {NULL};
	char *found;

	found = NULL;
	setup();
	bultin_export(input);
	bultin_export(empty_input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	found = strstr(out, "declare -x BANANA=\"banana\"");
	assert_true(found != NULL);
}

void	simple_export_2_term()
{
	char *input[]       = {"BANANA=banana", "ABACATE=abacate", NULL};
	char *empty_input[] = {NULL};
	char *banana;
	char *abacate;

	banana = NULL;
	abacate = NULL;
	setup();
	bultin_export(input);
	bultin_export(empty_input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	banana = strstr(out,  "declare -x BANANA=\"banana\"");
	abacate = strstr(out, "declare -x ABACATE=\"abacate\"");
	assert_true(banana != NULL && abacate != NULL);
}

void	export_empty_value()
{
	char *input[]       = {"limao", NULL};
	char *empty_input[] = {NULL};

	char *limao = NULL;
	setup();
	bultin_export(input);
	bultin_export(empty_input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	limao = strstr(out, "declare -x limao");
	assert_true(limao != NULL);	
}

void	export_empty_2_value()
{
	char *input[]       = {"limao", "abacaxi", NULL};
	char *empty_input[] = {NULL};

	char *limao = NULL;
	char *abacaxi = NULL;
	setup();
	bultin_export(input);
	bultin_export(empty_input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	limao = strstr(out, "declare -x limao");
	abacaxi = strstr(out, "declare -x abacaxi");
	assert_true(limao != NULL && abacaxi != NULL);	
}

void	export_3_values()
{
	char *input[]       = {"limao", "abacaxi", "fruta=morango", NULL};
	char *empty_input[] = {NULL};

	char *limao = NULL;
	char *abacaxi = NULL;
	char *morango = NULL;
	setup();
	bultin_export(input);
	bultin_export(empty_input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	limao =   strstr(out, "declare -x limao");
	abacaxi = strstr(out, "declare -x abacaxi");
	morango = strstr(out, "declare -x fruta=\"morango\"");
	assert_true(limao != NULL && abacaxi != NULL && morango != NULL);
}

void	export_error_1teste()
{
	char *input[]       = {"1teste", NULL};
	char *empty_input[] = {NULL};

	char *teste = NULL;
	setup();
	bultin_export(input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	assert_string_equal(out, "minishell: export: `1teste': not a valid identifier\n");
}

void	export_error_ok()
{
	char *input[]       = {"ok-", NULL};
	char *empty_input[] = {NULL};

	char *teste = NULL;
	setup();
	bultin_export(input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	assert_string_equal(out, "minishell: export: `ok-': not a valid identifier\n");
}

void	export_error_equal()
{
	char *input[]       = {"=teste", NULL};
	char *empty_input[] = {NULL};

	char *teste = NULL;
	setup();
	bultin_export(input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	assert_string_equal(out, "minishell: export: `=teste': not a valid identifier\n");
}

void	export_2_errors()
{
	char *input[]       = {"=", "=teste", NULL};
	char *empty_input[] = {NULL};

	char *teste = NULL;
	setup();
	bultin_export(input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	assert_string_equal(out, "minishell: export: `=': not a valid identifier\nminishell: export: `=teste': not a valid identifier\n");
}

void	export_2_args_one_error()
{
	char *input[]       = {"1_b", "teste=teste", NULL};
	char *empty_input[] = {NULL};

	char *teste = NULL;
	setup();
	bultin_export(input);
	teardown();
	char *out  = file_to_string(TEMP_FILENAME);
	setup();
	bultin_export(empty_input);
	teardown();
	char *out2 = file_to_string(TEMP_FILENAME);
	teste = strstr(out2,  "declare -x teste=\"teste\"");
	assert_string_equal(out, "minishell: export: `1_b': not a valid identifier\n");
	assert_true(teste != NULL);
	// free(out);
	// free(out2);
}

int	run_export_tests(void)
{
	const struct CMUnitTest export_tests[] = {
		cmocka_unit_test(simple_export_1_term),
		cmocka_unit_test(simple_export_2_term),
		cmocka_unit_test(export_empty_value),
		cmocka_unit_test(export_empty_2_value),
		cmocka_unit_test(export_3_values),
		cmocka_unit_test(export_error_1teste),
		cmocka_unit_test(export_error_ok),
		cmocka_unit_test(export_error_equal),
		cmocka_unit_test(export_2_errors),
		cmocka_unit_test(export_2_args_one_error),
	};
    return cmocka_run_group_tests(export_tests, NULL, NULL);
}