#include "tester.h"

int main(int argc, char *argv[], char *envp[]){
	init_env(envp);
	// run_echo_tests();
	run_export_tests();
	run_unset_tests();
	// run_cd_tests();
	// char *input[] = {"333", "32", NULL};
	// bultin_exit(input);
}