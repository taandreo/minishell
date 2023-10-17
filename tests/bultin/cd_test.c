#include "tester.h"

#define TEST_DIR "test_cd_dir"

void	cd_00()
{	
	char full_dir[4096];
	char *input[] = {TEST_DIR, NULL};
	char *parent_dir = getcwd(NULL, 0);
	bultin_cd(input);
	char *pwd = getcwd(NULL, 0);
	sprintf(full_dir, "%s/%s", parent_dir, TEST_DIR);
	assert_string_equal(full_dir, pwd);
	input[0] = parent_dir;
	bultin_cd(input);
	free(parent_dir);
	free(pwd);
}

void	cd_02()
{	
	char full_dir[4096];
	char *input[] = {"test_cd_dir/", NULL};
	char *parent_dir = getcwd(NULL, 0);
	bultin_cd(input);
	char *pwd = getcwd(NULL, 0);
	sprintf(full_dir, "%s/%s", parent_dir, TEST_DIR);
	assert_string_equal(full_dir, pwd);
	input[0] = parent_dir;
	bultin_cd(input);
	free(parent_dir);
	free(pwd);
}

void	cd_no_existing_dir()
{	
	char *input[] = {"no_existing_dir", NULL};
	setup();
	bultin_cd(input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	assert_string_equal(out, "minishell: cd: no_existing_dir: No such file or directory\n");
	free(out);
}

void	cd_to_a_file()
{
	char *input[] = {"isso_e_um_file.txt", NULL};
	empty_file("isso_e_um_file.txt");
	setup();
	bultin_cd(input);
	teardown();
	char *out = file_to_string(TEMP_FILENAME);
	assert_string_equal(out, "minishell: cd: isso_e_um_file.txt: Not a directory\n");
	free(out);
}

// void	cd_to_a_file()
// {
// 	char *input[] = {"no_permission_dir", NULL};
// 	empty_file("isso_e_um_file.txt");
// 	setup();
// 	bultin_cd(input);
// 	teardown();
// 	char *out = file_to_string(TEMP_FILENAME);
// 	assert_string_equal(out, "minishell: cd: no_permission_dir/: Permission denied\n");
// 	free(out);
// }

// void	check_PWD_and_OLDPWD()
// {
// 	char *empty_input = {NULL};
// 	char *input[] = {NULL};
// 	char *parent_dir = getcwd(NULL, 0);
// 	setup();
// 	bultin_cd(input);
// 	bultin_env(empty_input);
// 	teardown();
// 	char *out = file_to_string(TEMP_FILENAME);
// 	strstr(out, "PWD=");
// }

int	run_cd_tests(void)
{
	mkdir(TEST_DIR, S_IRWXU);
	const struct CMUnitTest export_tests[] = {
		cmocka_unit_test(cd_00),
		cmocka_unit_test(cd_02),
		cmocka_unit_test(cd_no_existing_dir),
		cmocka_unit_test(cd_to_a_file),
		// cmocka_unit_test(check_PWD_and_OLDPWD),
	};
    return cmocka_run_group_tests(export_tests, NULL, NULL);
}