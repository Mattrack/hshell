#ifndef __HSHELL_H__
#define __HSHELL_H__

#include <stdlib.h>
#include "utils/array.h"

#define UNUSED(x) (void)x


#define COMMAND_GET_MAXIMUM_CMD_SIZE 4096*4096

#define ENV_BUFFER_SIZE	1024
#define LINE_BUFFER_SIZE 4096
#define PROMPT_BUFFER_SIZE 1024
#define PATH_BUFFER_SIZE 1024
#define ENV_EXPAND_BUFFER_SIZE 1024
#define ARGV_BUFFER_SIZE 64
#define COMMAND_BUFFER_SIZE 16

#define SHELL_STATE_NONE		0
#define SHELL_STATE_INIT		1
#define SHELL_STATE_RUN			2
#define SHELL_STATE_CANCEL	4

#define SHELL_OP_NONE				0
#define SHELL_OP_AND				1
#define SHELL_OP_OR					2
#define SHELL_OP_ROOT				3

struct command_s {
	char 							**ARRAY(argv);
	struct command_s	**ARRAY(commands);
	int								op;
	void							*alias;
};

typedef struct command_s command_t;

struct command_chain_s {
	char			*ARRAY(line);
	command_t	root;
};

typedef struct command_chain_s command_chain_t;

struct shell_s {
	char						*ARRAY(line);
	char						*ARRAY(prompt);

	char						**ARRAY(env_keys);
	char						**ARRAY(env_values);
	char						**ARRAY(alias_keys);
	command_chain_t	**ARRAY(alias_commands);
	char						**history;
	int							history_write_index;
	int							history_size;
	int							history_count;

	int							exit;
	int							exit_code;
	char						**ARRAY(paths);
	char  					*paths_string;
	char						**ARRAY(envp);
	int 						state;
	int 						child_exit_code;

	int							is_tty;
	int							fd;
};


typedef struct shell_s shell_t;


#define OK 1

#define ERR_GET_COMMAND_EOF			0
#define ERR_GET_COMMAND_READ		-1
#define ERR_GET_COMMAND_TO_BIG	-2
#define ERR_GET_COMMAND_MEMORY	-3


int 			command_get(int fd, shell_t *shell);
void			command_chain_free(command_chain_t *command);
void			command_init(command_chain_t *command);
void			command_split(command_chain_t *command);
void			command_exec(shell_t *shell, command_chain_t *command);
int				command_builtins(shell_t *shell, command_t *command, int *status);
int				command_lexer(command_chain_t *command);
void			command_expand(shell_t *shell, command_chain_t *command);
void			command_remove_comment(command_chain_t *command);
void			command_remove_quote(command_chain_t *command);
void			command_free(command_t *command);
command_t *command_clone(command_t *command);

void	history_init(shell_t *shell, int size);
void 	history_add(shell_t *shell, const char *line);
void 	history_free(shell_t *shell);
void 	history_expand(shell_t *shell, command_chain_t* chain);
void	history_load(shell_t *shell);
void	history_save(shell_t *shell);
char	*history_get_file();

void	shell_init(shell_t *shell, int argc, char **argv, char **envp);
void	shell_free(shell_t *shell);
void	shell_getcwd(shell_t *shell);

void	prompt_print(shell_t *shell);
void	prompt_expand(shell_t* shell, const char* prompt);

const char			*util_get_home();
char						*util_read_file(const char* path);
const char			*util_get_user();
const char*			util_get_hostname();

command_chain_t	*alias_get(shell_t *shell, const char *key);
command_chain_t	*alias_set(shell_t *shell, char *alias, char *value);
void 						alias_expand(shell_t *shell, command_chain_t *chain);
int							alias_get_index(shell_t *shell, const char *key);
int							alias_get_index_n(shell_t *shell, const char *key, int n);
void						alias_add(shell_t *shell, char *env);

void	env_add(shell_t *shell, char *env);
int		env_remove(shell_t *shell, const char *key);
char	*env_get(shell_t *shell, const char *key);
char	*env_get_n(shell_t *shell, const char *key, int n);
int		env_get_index(shell_t *shell, const char *key);
int		env_get_index_n(shell_t *shell, const char *key, int n);
char	*env_set(shell_t *shell, const char *key, const char *value);
void	env_hook(shell_t *shell, const char *key);
void	env_rebuild_envp(shell_t *shell);
void	env_free_envp(shell_t *shell);
char	*env_expand(shell_t *shell, const char *str);

void	path_parse(shell_t *shell);
char	*path_expand(shell_t *shell, const char *value);
void	path_free(shell_t *shell);

#endif