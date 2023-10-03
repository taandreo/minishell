#include "minishell_bonus.h"

// Function to insert in a sorted manner
void sortedInsert(t_string **headRef, t_string *newNode) {
	t_string *current;
	if (*headRef == NULL || strcmp((*headRef)->value, newNode->value) >= 0) {
		newNode->next = *headRef;
		*headRef = newNode;
	} else {
		current = *headRef;
		while (current->next != NULL && strcmp(current->next->value, newNode->value) < 0) {
			current = current->next;
		}
		newNode->next = current->next;
		current->next = newNode;
	}
}

t_string *expand_wildcard(t_string *string, t_token_type type) {
	t_string *resultHead = NULL;

	if (!string)
		return (NULL);
	DIR *dir = opendir(".");
	if (!dir) {
		perror("opendir");
		return NULL;
	}
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		char *filename = entry->d_name;
		t_string *current = string;
		char *position = filename;
		int match = 1;  // assume a match by default

		while (current && match) {
			if (current->type == type) {
				char *found;
				if (current->next && current->next->type == TOKEN_WILDCARD) {
					// Match up to the next wildcard
					found = ft_strnstr(position, current->value, strlen(current->next->value));
				} else {
					found = strncmp(position, current->value, strlen(current->value)) == 0 ? position : NULL;
				}

				if (!found) {
					match = 0;
					break;
				} else {
					position = found + strlen(current->value);
				}
			} else if (current->type == TOKEN_WILDCARD) {
				// Move position after wildcard unless it's the last token
				if (current->next) {
					position++;
				} else {
					break;  // if it's the last token, match is guaranteed
				}
			}
			current = current->next;
		}

		if (match && (!position || *position == '\0' || (current && current->type == TOKEN_WILDCARD))) {
			t_string *newNode = malloc(sizeof(t_string));
			if (!newNode) {
				perror("malloc");
				closedir(dir);
				free_string(resultHead);  // clean up the already created nodes
				return NULL;
			}
			newNode->type = type;
			newNode->value = strdup(entry->d_name);
			sortedInsert(&resultHead, newNode);
		}
	}
	closedir(dir);
	return resultHead;
}
