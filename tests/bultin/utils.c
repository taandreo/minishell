#include "tester.h"

int original_stdout;
int	original_stderr;

void setup() {
	original_stdout = dup(STDOUT_FILENO);
	original_stderr = dup(STDERR_FILENO);
	// Open a file for redirection
    int output_fd = open(TEMP_FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("open");
        exit(1);
    }
    // Redirect stdout to the file descriptor
    if (dup2(output_fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(1);
    }
    // Redirect stderr to the file descriptor
    if (dup2(output_fd, STDERR_FILENO) == -1) {
        perror("dup2");
        exit(1);
    }
	close(output_fd);
}

void teardown() {
	fflush(stdout);
    if (dup2(original_stdout, STDERR_FILENO) == -1) {
        perror("dup2");
        exit(1);
    }
	if (dup2(original_stderr, STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(1);
    }
}

char *file_to_string(const char *filename) {
    FILE *file = fopen(filename, "rb"); // Open the file in binary read mode
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fileSize < 0) {
        perror("Failed to determine file size");
        fclose(file);
        return NULL;
    }

    // Allocate memory for the string
    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read the file content into the buffer
    long bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead != fileSize) {
        perror("Failed to read file");
        fclose(file);
        free(buffer);
        return NULL;
    }

    // Null-terminate the string
    buffer[fileSize] = '\0';

    // Close the file
    fclose(file);

    return buffer;
}
