#include "main.h"

/**
 * main - Entry point for a simple shell program.
 * @arg_c: The number of command-line arguments.
 * @arg_v: An array of command-line argument strings.
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int main(int arg_c, char **arg_v)
{
	int file_desc = 2; /* File descriptor initialized to 2 (stderr) */
	info_t info_struct[] = { INFO_INIT }; /* Initialize info_struct structure */

	/* Inline assembly code to modify the file descriptor */
	asm (
		"mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (file_desc)
		: "r" (file_desc)
	);

	if (arg_c == 2)
	{
		/* Attempt to open the file specified in the command-line argument */
		file_desc = open(arg_v[1], O_RDONLY);

		if (file_desc == -1)
		{
			if (errno == EACCES)
				exit(126); /* Exit with status 126 on permission denied */
			if (errno == ENOENT)
			{
				print_string(arg_v[0]);
				print_string(": 0: Can't open ");
				print_string(arg_v[1]);
				print_char('\n');
				print_char(BUF_FLUSH);
				exit(127); /* Exit with status 127 on file not found */
			}
			return (EXIT_FAILURE); /* Exit with generic failure status */
		}
		info_struct->readfd = file_desc;
	}

	/* Populate environment list, read history, and execute shell logic */
	initialize_env_list(info_struct);
	read_command_history(info_struct);
	hsh_loop(info_struct, arg_v);

	return (EXIT_SUCCESS); /* Exit with success status */
}

