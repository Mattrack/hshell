#include <hshell.h>
#include "utils/hlib.h"

void history_add_r(shell_t* shell, const char* line) {
	int		current_index	= ((shell->history_write_index - 1) + shell->history_size) % (shell->history_size);
	char	*current 			= shell->history[current_index];	
	
	if ((current == NULL) || (hstrcmp(current, line) != 0)) {		
		free(shell->history[shell->history_write_index]);	
		shell->history[shell->history_write_index] = hstrdup(line);		
		shell->history_write_index = (shell->history_write_index + 1) % (shell->history_size);
		shell->history_count++;
	}
	if (shell->history_count > shell->history_size) {
		shell->history_count = shell->history_size;
	}
}

void history_add(shell_t *shell, const char	*line) {
	char	*token;
	char	*saveptr;
	char	*tmp = hstrdup(line); 

	token = hstrtok_r(tmp, "\n", &saveptr);
	while (token) {
		history_add_r(shell, token);
		token = hstrtok_r(NULL, "\n", &saveptr);
	}
	free(tmp);
}