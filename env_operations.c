#include "main.h"

/**
 * get_environ - Retrieves the environment variables as an array of strings.
 * @info_struct: Pointer to the information structure.
 *
 * Return: Pointer to an array of strings representing environment variables.
 *
 * Description: This function retrieves the environment variables stored in
 * the
 * 'info_struct' as an array of strings. If the 'env_changed' flag is set or
 * 'environ' is not yet populated, the function converts the linked list of
 * environment variables into an array of strings using the
 * 'linked_list_to_strings' function.
 * The 'env_changed' flag is then reset. The resulting array is
 * returned.
 */
char **get_environ(info_t *info_struct)
{
	if (!info_struct->environ || info_struct->env_changed)
	{
		info_struct->environ = linked_list_to_strings(info_struct->env);
		info_struct->env_changed = 0;
	}

	return (info_struct->environ);
}

/**
 * _unset_env - Unsets an environment variable by name.
 * @info_struct: Pointer to the information structure.
 * @char_var: Pointer to the variable name.
 *
 * Return: 1 if environment changed, 0 otherwise.
 *
 * Description: This function unsets an environment variable specified by
 * 'var'.
 * It iterates through the environment variable list and searches for
 * variables with the same name as 'var'. When found, the variable is removed
 * from the list.
 * The function manages memory and updates the 'env_changed' flag in the
 * 'info_struct' to indicate environment changes. It returns 1 if environment
 * changes were made, otherwise, it returns 0.
 */
int _unset_env(info_t *info_struct, char *char_var)
{
	info_list *current_node = info_struct->env;
	size_t count = 0;
	char *char_ptr;

	if (!current_node || !char_var)
	{
		return (0);
	}

	while (current_node)
	{
		char_ptr = starts_with(current_node->str, char_var);
		if (char_ptr && *char_ptr == '=')
		{
			info_struct->env_changed = delete_node_at_index(&(info_struct->env), count);
			count = 0;
			current_node = info_struct->env;
			continue;
		}
		current_node = current_node->next;
		count++;
	}
	return (info_struct->env_changed);
}

/**
 * _setenv - Sets an environment variable with a given value.
 * @info_struct: Pointer to the information structure.
 * @char_var: Pointer to the variable name.
 * @value: Pointer to the value to set.
 *
 * Return: 0 on success, 1 on memory allocation error.
 *
 * Description: This function sets an environment variable 'var' with the
 * specified 'value'.
 * It first allocates memory for the new variable using 'malloc'.
 * The new variable is created
 * in the format 'var=value'. If a variable with the same name already
 * exists in the environment, its value is updated. If not, a new variable
 * is added to the environment list. The function manages memory and
 * environment changes, updating 'env_changed' in the 'info_struct' to indicate
 * changes. The return value is 0 on success or 1 on a memory allocation error.
 */
int _setenv(info_t *info_struct, char *char_var, char *value)
{
	char *buffer = NULL;
	info_list *node_cursor;
	char *start_char;

	if (!char_var || !value)
	{
		return (0);
	}

	buffer = malloc(_strlen(char_var) + _strlen(value) + 2);
	if (!buffer)
	{
		return (1);
	}
	_strcpy(buffer, char_var);
	_strcat(buffer, "=");
	_strcat(buffer, value);

	node_cursor = info_struct->env;
	while (node_cursor)
	{
		start_char = starts_with(node_cursor->str, char_var);
		if (start_char && *start_char == '=')
		{
			free(node_cursor->str);
			node_cursor->str = buffer;
			info_struct->env_changed = 1;
			return (0);
		}
		node_cursor = node_cursor->next;
	}

	add_node_end(&(info_struct->env), buffer, 0);
	free(buffer);
	info_struct->env_changed = 1;
	return (0);
}

