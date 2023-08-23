#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>


#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".hsh_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - A structure to represent a linked list of strings
 * @num: Numeric identifier associated with the node
 * @str: Pointer to a string stored in the node
 * @next: Pointer to the next node in the linked list
 *
 * The `liststr` struct is used to create a linked list of strings,
 * with each node containing a numeric identifier, a string, and a
 * pointer to the next node in the list.
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} info_list;

/**
 * struct passinfo - A structure to hold information for a shell program
 * @arg: Argument string containing the input command
 * @argv: Array of strings containing the individual arguments of the command
 * @path: Pointer to the PATH environment variable
 * @argc: Number of arguments in the @argv array
 * @line_count: Current line number in the input
 * @err_num: Error number associated with the command
 * @linecount_flag: Flag indicating whether line count should be displayed
 * @fname: Name of the file being processed
 * @env: Pointer to a linked list of environment variables
 * @history: Pointer to a linked list of command history
 * @alias: Pointer to a linked list of command aliases
 * @environ: Array of strings representing the environment variables
 * @env_changed: Flag indicating if environment variables have changed
 * @status: Status code of the last executed command

 * @cmd_buf: Pointer to a command chain buffer for memory management
 * @cmd_buf_type: Type of command chain (CMD_type: ||, &&, ;)
 * @readfd: File descriptor for reading input
 * @histcount: Count of command history
 *
 * The `passinfo` struct holds various fields and pointers to manage
 * information related to a shell program. It contains details about
 * command arguments, environment variables, command history, and more.
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	info_list *env;
	info_list *history;
	info_list *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf;
	int cmd_buf_type;
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 * struct builtin - A structure to hold information about built-in commands
 * @type: String representing the name of the built-in command
 * @func: Pointer to the function implementing the built-in command
 *
 * The `builtin` struct holds information about built-in commands in a shell program.
 * It includes the name of the built-in command and a pointer to the function that
 * implements its functionality.
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


int hsh_loop(info_t *, char **);
int search_builtin(info_t *);
void search_exe_cmd(info_t *);
void fork_execute_cmd(info_t *);

int is_exe_cmd(info_t *, char *);
char *duplicate_chars(char *, int, int);
char *find_cmd_path(info_t *, char *, char *);

int print_hsh_alias(info_list *node);
int set_hsh_alias(info_t *info_struct, char *str);
int unset_hsh_alias(info_t *info_struct, char *str);

int loophsh(char **);

void print_string(char *);
int print_char(char);
int print_char_file_desc(char c, int file_desc);
int print_string_file_desc(char *str, int file_desc);

int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

char **strtow(char *, char *);
char **strtow2(char *, char);

char *_memset(char *, char, unsigned int);
void free_ptr_array(char **);
void *_realloc(void *, unsigned int, unsigned int);

int free_ptr(void **);

int is_interactive(info_t *);
int is_delimiter(char, char *);
int is_alpha(int);
int convert_str_to_int(char *);

int parse_unsigned_Int(char *);
void print_error(info_t *, char *);
int print_decimal(int, int);
char *num_str_converter(long int, int, int);
void remove_comments(char *);

int hsh_exit(info_t *);
int hsh_cd(info_t *);
int hsh_help(info_t *);

int hsh_history(info_t *);
int hsh_alias(info_t *);

ssize_t get_input(info_t *);
int get_stdin_line(info_t *, char **, size_t *);
ssize_t read_buffer(info_t *, char *, size_t *);
ssize_t input_buffer(info_t *, char **, size_t *);
void sigint_handler(__attribute__((unused)) int signal_num);

void clear_info_list(info_t *);
void set_info_list(info_t *, char **);
void free_info_list(info_t *, int);

char *get_env_var(info_t *, const char *);
int display_env_var(info_t *);
int update_env_var(info_t *);
int clear_all_env_var(info_t *);
int initialize_env_list(info_t *);

char **get_environ(info_t *);
int _unset_env(info_t *, char *);
int _setenv(info_t *, char *, char *);

char *generate_history_filepath(info_t *info_struct);
int write_history_to_file(info_t *info_struct);
int read_command_history(info_t *info_struct);
int add_to_history_list(info_t *info_struct, char *buffer, int line_count);
int add_numbering_to_history(info_t *info_struct);

info_list *add_node(info_list **, const char *, int);
info_list *add_node_end(info_list **, const char *, int);
size_t print_linked_list_str(const info_list *);
int delete_node_at_index(info_list **, unsigned int);
void free_list(info_list **);

size_t linked_list_len(const info_list *);
char **linked_list_to_strings(info_list *);
size_t print_linked_list(const info_list *);
info_list *node_starts_with(info_list *, char *, char);
ssize_t get_node_index(info_list *, info_list *);

int is_chain_delimiter(info_t *, char *, size_t *);
void check_cmd_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_cmd_with_alias(info_t *);
int replace_cmd_argv(info_t *);
int replace_string(char **, char *);

#endif

