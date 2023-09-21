#include "tester.h"

static FILE* original_stdout = NULL;
static FILE* temp_stdout = NULL;

void setup() {
    original_stdout = stdout;
    temp_stdout = fopen(temp_filename, "w");
    assert_non_null(temp_stdout);
    stdout = temp_stdout;
}

void teardown() {
    if (temp_stdout) {
        fclose(temp_stdout);
        stdout = original_stdout;
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