#include "main.h"

/**
 * clear_info_list - Clears fields within the info_t structure.
 * @info_struct: Pointer to the info_t structure.
 *
 * Description: This function resets various fields within the info_t structure
 * to initial values. It sets the arg, argv, and path fields to NULL, and the
 * argc field to 0, effectively clearing any previous data stored in these
 * fields.
 */
void clear_info_list(info_t *info_struct)
{
	info_struct->arg = NULL;
	info_struct->argv = NULL;
	info_struct->path = NULL;
	info_struct->argc = 0;
}

/**
 * set_info_list - Sets up info_t structure using command arguments.
 * @info_struct: Pointer to the info_t structure.
 * @arg_v: Array of command arguments.
 *
 * Description: This function initializes various fields within the info_t
 * structure using the provided array of command arguments. It sets the fname
 * field to the first argument (program name) in arg_v. It then populates the
 * argv field with the result of splitting the arg field (if not NULL) by
 * delimiters " \t". If argv allocation or splitting fails, a fallback is used
 * to allocate space for argv. The argc field is set to the count of arguments
 * in argv. The function then calls replace_cmd_with_alias and replace_cmd_argv
 * to perform
 * alias and variable replacement.
 */
void set_info_list(info_t *info_struct, char **arg_v)
{
	int count = 0;

	info_struct->fname = arg_v[0];
	if (info_struct->arg)
	{
		info_struct->argv = strtow(info_struct->arg, " \t");
		if (!info_struct->argv)
		{
			info_struct->argv = malloc(sizeof(char *) * 2);
			if (info_struct->argv)
			{
				info_struct->argv[0] = _strdup(info_struct->arg);
				info_struct->argv[1] = NULL;
			}
		}
		for (count = 0; info_struct->argv && info_struct->argv[count]; count++)
			;
		info_struct->argc = count;

		replace_cmd_with_alias(info_struct);
		replace_cmd_argv(info_struct);
	}
}

/**
 * free_info_list - Frees memory allocated for the info_t structure.
 * @info_struct: Pointer to the info_t structure.
 * @free_all: Flag indicating whether to free all components (1) or not (0).
 *
 * Description: This function releases the memory allocated for various
 * components within the info_t structure. If 'all' is 1, it frees all
 * components including argv, path, arg (if cmd_buf is NULL), env, history,
 * alias, environ, cmd_buf, and closes the read file descriptor. If 'all' is 0,
 * it only frees argv and sets path to NULL.
 */
void free_info_list(info_t *info_struct, int free_all)
{
	free_ptr_array(info_struct->argv);
	info_struct->argv = NULL;
	info_struct->path = NULL;
	if (free_all)
	{
		if (!info_struct->cmd_buf)
		{
			free(info_struct->arg);
		}
		if (info_struct->env)
		{
			free_list(&(info_struct->env));
		}
		if (info_struct->history)
		{
			free_list(&(info_struct->history));
		}
		if (info_struct->alias)
		{
			free_list(&(info_struct->alias));
		}
		free_ptr_array(info_struct->environ);
		info_struct->environ = NULL;
		free_ptr((void **)info_struct->cmd_buf);
		if (info_struct->readfd > 2)
		{
			close(info_struct->readfd);
		}
		_putchar(BUF_FLUSH);
	}
}


/**
 * _strncpy - Copies a specified number of characters from one string to
 * another.
 * @dest_char: Pointer to the destination string.
 * @src_char: Pointer to the source string.
 * @num_char: Number of characters to copy from 'src_char' to 'dest_char'.
 *
 * Return: Pointer to the copied string 'dest_char'.
 *
 * Description: This function copies at most 'num_char' characters from the
 * string pointed to by 'src_char' to the string pointed to by 'dest_char'. If
 * 'src_char' is shorter than 'num_char' characters, null bytes are appended to
 * the end of 'dest_char' to ensure a total of 'num_char' characters. The
 * function then returns a pointer to the resulting copied string 'dest_char'.
 */
char *_strncpy(char *dest_char, char *src_char, int num_char)
{
	int count_i, count_j;
	char *dest_char_temp = dest_char;

	count_i = 0;
	while (src_char[count_i] != '\0' && count_i < num_char - 1)
	{
		dest_char[count_i] = src_char[count_i];
		count_i++;
	}
	if (count_i < num_char)
	{
		count_j = count_i;
		while (count_j < num_char)
		{
			dest_char[count_j] = '\0';
			count_j++;
		}
	}
	return (dest_char_temp);
}
