#include "main.h"

/**
 * hsh_history - Displays the command history.
 * @info_struct: Pointer to the info structure.
 *
 * Return: Always returns 0.
 *
 * Description: This function displays the command history stored in the
 * history linked list of the info structure. It uses the print_linked_list
 * function to print each history entry in the list. The function always
 * returns 0 to indicate success.
 */
int hsh_history(info_t *info_struct)
{
	print_linked_list(info_struct->history); /* Print the command history */
	return (0);						  /* Return 0 to indicate success */
}

/**
 * hsh_exit - Handles the exit command.
 * @info_struct: Pointer to the info structure.
 *
 * Return: 1 on error, -2 to indicate exit with a specific status, or -1
 * otherwise.
 *
 * Description: This function handles the exit command. If an exit argument
 * is provided, it attempts to convert it to an integer and sets the error
 * number accordingly. If the conversion fails, an error message is printed.
 * The function returns 1 on error, -2 to indicate exit with a specific status,
 * or -1 otherwise. It sets the error number in the info structure to indicate
 * whether an exit status is provided or not.
 */
int hsh_exit(info_t *info_struct)
{
	int error_code;

	if (info_struct->argv[1]) /* If there is an exit argument */
	{
		error_code = parse_unsigned_Int(info_struct->argv[1]);

		if (error_code == -1)
		{
			info_struct->status = 2;
			print_error(info_struct, "Illegal number: ");
			print_string(info_struct->argv[1]);
			print_char('\n');
			return (1);
		}

		info_struct->err_num = parse_unsigned_Int(info_struct->argv[1]);
		return (-2);
	}

	info_struct->err_num = -1;
	return (-2);
}

/**
 * hsh_cd - Handles the cd (change directory) command.
 * @info_struct: Pointer to the info structure.
 *
 * Return: 0 on success, 1 on error.
 *
 * Description: This function handles the cd command, which is used to change
 * the current working directory. If no argument is provided, it attempts to
 * change the directory to the user's home directory. If the argument is "-",
 * it changes the directory to the previous working directory. Otherwise, it
 * attempts to change the directory to the specified path. The function updates
 * the PWD and OLDPWD environment variables accordingly.
 */
int hsh_cd(info_t *info_struct)
{
	char *current_dir, *dir, buffer[1024];
	int ch_d;

	current_dir = getcwd(buffer, 1024);
	if (!current_dir)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info_struct->argv[1]) /* No argument - change to home directory */
	{
		dir = get_env_var(info_struct, "HOME=");
		if (!dir)
			ch_d = chdir((dir = get_env_var(info_struct, "PWD=")) ? dir : "/");
		else
			ch_d = chdir(dir);
	}
	else if (_strcmp(info_struct->argv[1], "-") == 0)
	{
		if (!get_env_var(info_struct, "OLDPWD="))
		{
			_puts(current_dir);
			_putchar('\n');
			return (1);
		}
		_puts(get_env_var(info_struct, "OLDPWD=")), _putchar('\n');
		ch_d = chdir((dir = get_env_var(info_struct, "OLDPWD=")) ? dir : "/");
	}
	else /* Change to specified directory */
		ch_d = chdir(info_struct->argv[1]);
	if (ch_d == -1) /* Handle chdir error */
	{
		print_error(info_struct, "can't cd to ");
		print_string(info_struct->argv[1]), print_char('\n');
	}
	else
	{
		_setenv(info_struct, "OLDPWD", get_env_var(info_struct, "PWD="));
		_setenv(info_struct, "PWD", getcwd(buffer, 1024));
	}
	return (0); /* Return 0 on success */
}

/**
 * hsh_help - Handles the help command.
 * @info_struct: Pointer to the info structure.
 *
 * Return: Always returns 0.
 *
 * Description: This function handles the help command, which is used to
 * display information about shell commands. Currently, the function is
 * not yet implemented, but it displays a placeholder message.
 * The function always returns 0.
 */
int hsh_help(info_t *info_struct)
{
	char **arg_list;

	arg_list = info_struct->argv;
	_puts("Help function handled but not yet implemented \n");

	if (0)
		_puts(*arg_list); /* temp att_unused workaround */

	return (0); /* Always return 0 */
}
