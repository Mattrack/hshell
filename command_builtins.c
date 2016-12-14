#include <unistd.h>
#include "hshell.h"
#include "hlib.h"

struct builtin_s {
	char	*name;
	void	(*fct)(shell_t *shell, command_t *command);
};


void builtin_env(shell_t *shell, command_t *cmd) {
	char	**envp = shell->envp;
	char	*env_filter = cmd->argv_size > 2 ? cmd->argv[1] : NULL; 
	int		env_filter_len = env_filter ? hstrlen(env_filter) : 0;
	while (*envp) {
		if ( (env_filter_len == 0) || (hstrncmp(env_filter, *envp, env_filter_len) == 0)) {
			hprintf(*envp);
			hprintf("\n");
		}
		envp++;
	}
	UNUSED(cmd);
}

void builtin_setenv(shell_t *shell, command_t *cmd) {
	if (cmd->argv_size > 3) {
		env_set(shell, cmd->argv[1], cmd->argv[2]);
	} else {
		hprintf("missing argument\n");
	}
}

void builtin_exit(shell_t* shell, command_t* cmd) {
	shell->exit = 1;
	if (cmd->argv_size > 2) {
		shell->exit_code = atoi(cmd->argv[1]);
	}
}

void builtin_unsetenv(shell_t *shell, command_t *cmd) {
	if (cmd->argv_size > 2) {
		env_remove(shell, cmd->argv[1]);
	} else {
		hprintf("missing argument\n");
	}
}

void builtin_cd(shell_t *shell, command_t *cmd) {
	if (cmd->argv_buffer_size > 2) {
		char* path = cmd->argv[1]; 
		if (hstrcmp(cmd->argv[1], "-") == 0) {
			if (shell->previous_pwd) {
				path = shell->previous_pwd; 
			} else {
				path = ".";
			}
		}
		chdir(path);
	}
	shell_getcwd(shell);
}

static struct builtin_s s_builtins[] = {
	{"exit", builtin_exit},
	{"env", builtin_env},
	{"setenv", builtin_setenv},
	{"unsetenv", builtin_unsetenv},
	{"cd", builtin_cd},
	{NULL, NULL}
};


int command_builtins(shell_t *shell, command_t *command) {
	int index = 0;
	while (s_builtins[index].name) {
		if (hstrcmp(s_builtins[index].name, command->argv[0]) == 0) {
			s_builtins[index].fct(shell, command);
			return 1;
		}
		index++;
	}
	return 0;
}