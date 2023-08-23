#include "main.h"

/**
 * hsh_loop - Main loop of the shell program.
 * @info_struct: Pointer to the info_struct structure.
 * @arg_v: An array of command-line argument strings.
 *
 * Return: The exit status of the last executed command.
 *
 * Description: This function constitutes the main loop of the shell program.
 * It continuously reads user input, processes it, and executes commands until
 * an exit condition is met. The loop first clears the info_struct structure,
 * checks if shell is running in an is_interactive mode, and prompts for input.
 * It then reads input, sets up the info_struct structure, and checks for
 * built-in commands. If no built-in command found, it searches for external
 * commands.
 * The loop also handles error conditions, freeing resources, and writing
 * history to a file.
 * The function returns the exit status of the last executed command.
 */
int hsh_loop(info_t *info_struct, char **arg_v)
{
	int builtin_result = 0;
	ssize_t read_result = 0;

	while (read_result != -1 && builtin_result != -2)
	{
		clear_info_list(info_struct);
		if (is_interactive(info_struct))
			_puts("$ ");
		print_char(BUF_FLUSH);
		read_result = get_input(info_struct);

		if (read_result != -1)
		{
			set_info_list(info_struct, arg_v);
			builtin_result = search_builtin(info_struct);

			if (builtin_result == -1)
				search_exe_cmd(info_struct);
		}
		else if (is_interactive(info_struct))
			_putchar('\n');

		free_info_list(info_struct, 0);
	}
	write_history_to_file(info_struct);
	free_info_list(info_struct, 1);

	if (!is_interactive(info_struct) && info_struct->status)
		exit(info_struct->status);

	if (builtin_result == -2)
	{
		if (info_struct->err_num == -1)
			exit(info_struct->status);
		exit(info_struct->err_num);
	}

	return (builtin_result);
}


/**
 * search_builtin - Searches for and executes a built-in command.
 * @info_struct: Pointer to the info structure.
 *
 * Return: The return value of the executed built-in command,
 * or -1 if not found.
 *
 * Description: This function searches for a built-in command using the
 * provided command name in the info structure's arguments. If a match
 * is found in the built-in command table, the corresponding built-in function
 * is executed with the info structure passed as a parameter. The function
 * returns the return value of the executed built-in command,
 * or -1 if the command is not found.
 */
int search_builtin(info_t *info_struct)
{
	int count, built_in_command = -1;
	builtin_table builtintbl[] = {
		{"exit", hsh_exit},
		{"env", display_env_var},
		{"help", hsh_help},
		{"history", hsh_history},
		{"setenv", update_env_var},
		{"unsetenv", clear_all_env_var},
		{"cd", hsh_cd},
		{"alias", hsh_alias},
		{NULL, NULL}
	};

	for (count = 0; builtintbl[count].type; count++)
	{
		if (_strcmp(info_struct->argv[0], builtintbl[count].type) == 0)
		{
			info_struct->line_count++;
			built_in_command = builtintbl[count].func(info_struct);
			break;
		}
	}

	return (built_in_command);
}


/**
 * search_exe_cmd - Searches for and executes an external command.
 * @info_struct: Pointer to the info structure.
 *
 * Description: This function searches for an external command using the
 * provided command name in the info structure's arguments. It first checks
 * if the command name contains a valid path. If not, it attempts to find the
 * command in the paths specified in the PATH environment variable. If the
 * command is found, the function forks a child process to execute the command.
 * If the command is not found, an error message is printed.
 */
void search_exe_cmd(info_t *info_struct)
{
	int count_i, count_k;
	char *env_path = NULL;

	info_struct->path = info_struct->argv[0];

	if (info_struct->linecount_flag == 1)
	{
		info_struct->line_count++;
		info_struct->linecount_flag = 0;
	}

	for (count_i = 0, count_k = 0; info_struct->arg[count_i]; count_i++)
		if (!is_delimiter(info_struct->arg[count_i], " \t\n"))
			count_k++;

	if (!count_k)
		return;

	env_path = find_cmd_path(info_struct, get_env_var(info_struct, "PATH="), info_struct->argv[0]);

	if (env_path)
	{
		info_struct->path = env_path;
		fork_execute_cmd(info_struct);
	}
	else
	{
		if ((is_interactive(info_struct) || get_env_var(info_struct, "PATH=") || info_struct->argv[0][0] == '/')
			&& is_exe_cmd(info_struct, info_struct->argv[0]))
			fork_execute_cmd(info_struct);
		else if (*(info_struct->arg) != '\n')
		{
			info_struct->status = 127;
			print_error(info_struct, "not found\n");
		}
	}
}

/**
 * fork_execute_cmd - Forks a child process to execute a command.
 * @info_struct: Pointer to the info structure.
 *
 * Description: This function forks a child process to execute a command
 * specified by the path and arguments in the info structure. The child
 * process uses the execve system call to replace its image with the
 * specified command. If the fork fails, an error message is printed.
 * If the execve call fails, the function frees resources,
 * handles error conditions, and exits with appropriate status codes.
 */
void fork_execute_cmd(info_t *info_struct)
{
	pid_t child_process_pid;

	child_process_pid = fork();
	if (child_process_pid == -1)
	{
		perror("Error:"); /* Print error message */
		return;
	}

	if (child_process_pid == 0) /* Child process */
	{
		if (execve(info_struct->path, info_struct->argv,
		get_environ(info_struct)) == -1)
		{
			free_info_list(info_struct, 1);

			if (errno == EACCES)
				exit(126);
			
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else /* Parent process */
	{
		/* Wait for the child process to complete */
		wait(&(info_struct->status));

		/* Check if the child process exited normally */
		if (WIFEXITED(info_struct->status))
		{
			info_struct->status = WEXITSTATUS(info_struct->status);

			if (info_struct->status == 126)
				print_error(info_struct, "Permission denied\n");
		}
	}
}
