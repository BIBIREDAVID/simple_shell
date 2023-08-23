#ifndef _MAIN_H_
#define _MAIN_H_

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for num_str_converter() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} info_list;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
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

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* toem_shloop.c */
int hsh_loop(info_t *, char **);
int search_builtin(info_t *);
void search_exe_cmd(info_t *);
void fork_execute_cmd(info_t *);

/* toem_parser.c */
int is_exe_cmd(info_t *, char *);
char *duplicate_chars(char *, int, int);
char *find_cmd_path(info_t *, char *, char *);

/* for builin_env_alias */
int print_hsh_alias(info_list *node);
int set_hsh_alias(info_t *info_struct, char *str);
int unset_hsh_alias(info_t *info_struct, char *str);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void print_string(char *);
int print_char(char);
int print_char_file_desc(char c, int file_desc);
int print_string_file_desc(char *str, int file_desc);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void free_ptr_array(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int free_ptr(void **);

/* toem_atoi.c */
int is_interactive(info_t *);
int is_delimiter(char, char *);
int is_alpha(int);
int convert_str_to_int(char *);

/* toem_errors1.c */
int parse_unsigned_Int(char *);
void print_error(info_t *, char *);
int print_decimal(int, int);
char *num_str_converter(long int, int, int);
void remove_comments(char *);

/* toem_builtin.c */
int hsh_exit(info_t *);
int hsh_cd(info_t *);
int hsh_help(info_t *);

/* toem_builtin1.c */
int hsh_history(info_t *);
int hsh_alias(info_t *);

/*toem_getline.c */
ssize_t get_input(info_t *);
int get_stdin_line(info_t *, char **, size_t *);
ssize_t read_buffer(info_t *, char *, size_t *);
ssize_t input_buffer(info_t *, char **, size_t *);
void sigint_handler(__attribute__((unused)) int signal_num);

/* toem_getinfo.c */
void clear_info_list(info_t *);
void set_info_list(info_t *, char **);
void free_info_list(info_t *, int);

/* toem_environ.c */
char *get_env_var(info_t *, const char *);
int display_env_var(info_t *);
int update_env_var(info_t *);
int clear_all_env_var(info_t *);
int initialize_env_list(info_t *);

/* toem_getenv.c */
char **get_environ(info_t *);
int _unset_env(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* toem_history.c */
char *generate_history_filepath(info_t *info_struct);
int write_history_to_file(info_t *info_struct);
int read_command_history(info_t *info_struct);
int add_to_history_list(info_t *info_struct, char *buffer, int line_count);
int add_numbering_to_history(info_t *info_struct);

/* toem_lists.c */
info_list *add_node(info_list **, const char *, int);
info_list *add_node_end(info_list **, const char *, int);
size_t print_linked_list_str(const info_list *);
int delete_node_at_index(info_list **, unsigned int);
void free_list(info_list **);

/* toem_lists1.c */
size_t linked_list_len(const info_list *);
char **linked_list_to_strings(info_list *);
size_t print_linked_list(const info_list *);
info_list *node_starts_with(info_list *, char *, char);
ssize_t get_node_index(info_list *, info_list *);

/* toem_vars.c */
int is_chain_delimiter(info_t *, char *, size_t *);
void check_cmd_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_cmd_with_alias(info_t *);
int replace_cmd_argv(info_t *);
int replace_string(char **, char *);

#endif

