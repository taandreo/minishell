#include "tester.h"

int main(int argc, char *argv[], char *envp[]){
	init_env(envp);
	// run_echo_tests();
	// run_export_tests();
	// run_unset_tests();
	// run_cd_tests();
	char *input[] = {"no_permission_dir/", NULL};
	char *empty_input[] = {NULL};
	bultin_cd(input);
}