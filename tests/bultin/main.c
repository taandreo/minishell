#include "tester.h"

int main(int argc, char *argv[], char *envp[]){
	init_env(envp);
	run_echo_tests();
	run_export_tests();
}

// int main(int argc, char *argv[], char *envp[]){
// 	char *input[]       = {"1teste", NULL};
// 	char *empty_input[] = {NULL};

// 	setup();
// 	bultin_export(input);
// 	teardown();
// 	// char *out = file_to_string(TEMP_FILENAME);
// 	// printf("%s", out);
// }