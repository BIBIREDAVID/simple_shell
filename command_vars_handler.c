#include "main.h"

/**
 * is_chain_delimiter - Check if the current position in the buffer is a
 * command chain delimiter.
 * @info_struct: Pointer to the info_t structure.
 * @buffer: The input buffer containing the command chain.
 * @pos_ptr: Pointer to the current position in the buffer.
 *
 * Description:
 * This function checks if the current position in the buffer corresponds to a
 * command chain delimiter such as "||", "&&", or ";". If a delimiter is found,
 * it updates the buffer to replace the delimiter with a null terminator and
 * updates the command buffer type in the info_struct.
 *
 * Return: 1 if a delimiter is found, 0 otherwise.
 */
int is_chain_delimiter(info_t *info_struct, char *buffer, size_t *pos_ptr)
{
	size_t pos_ptr_temp = *pos_ptr;

	if (buffer[pos_ptr_temp] == '|' && buffer[pos_ptr_temp + 1] == '|')
	{
		buffer[pos_ptr_temp] = 0;
		pos_ptr_temp++;
		info_struct->cmd_buf_type = CMD_OR;
	}
	else if (buffer[pos_ptr_temp] == '&' && buffer[pos_ptr_temp + 1] == '&')
	{
		buffer[pos_ptr_temp] = 0;
		pos_ptr_temp++;
		info_struct->cmd_buf_type = CMD_AND;
	}
	else if (buffer[pos_ptr_temp] == ';') /* found end of this command */
	{
		buffer[pos_ptr_temp] = 0; /* replace semicolon with null */
		info_struct->cmd_buf_type = CMD_CHAIN;
	}
	else
	{
		return (0);
	}
	*pos_ptr = pos_ptr_temp;
	return (1);
}


/**
 * check_cmd_chain - Check the command chain type and update buffer position if
 * needed.
 * @info_struct: Pointer to the info_t structure.
 * @buffer: The input buffer containing the command chain.
 * @ptr: Pointer to the current position in the buffer.
 * @start: Start position of the current command.
 * @buffer_len: Length of the input buffer.
 *
 * Description:
 * This function checks the type of command chain specified in info_struct and
 * updates the buffer position if necessary. If the command chain is of type
 * CMD_AND and the previous command's exit status is non-zero, the buffer is
 * truncated to remove the current command and any remaining commands. If the
 * command chain is of type CMD_OR and the previous command's exit status is
 * zero, the buffer is similarly truncated to remove the current command and
 * any remaining commands.
 *
 * Return: None.
 */
void check_cmd_chain(info_t *info_struct, char *buffer, size_t *ptr,
		size_t start, size_t buffer_len)
{
	size_t ptr_temp = *ptr;

	if (info_struct->cmd_buf_type == CMD_AND)
	{
		if (info_struct->status)
		{
			buffer[start] = 0;
			ptr_temp = buffer_len;
		}
	}
	if (info_struct->cmd_buf_type == CMD_OR)
	{
		if (!info_struct->status)
		{
			buffer[start] = 0;
			ptr_temp = buffer_len;
		}
	}

	*ptr = ptr_temp;
}

/**
 * replace_cmd_with_alias - Replace the command with its alias.
 * @info_struct: Pointer to the info_t structure.
 *
 * Description:
 * This function replaces the command in info_struct->argv[0] with its
 * corresponding alias from the alias list.
 * It iterates through the alias list and checks if the current command starts
 * with an alias name followed by '='. If a matching alias is found, the
 * command is replaced with the alias value (excluding the '=').
 *
 * Return:
 * - 0 on failure or if no matching alias is found.
 * - 1 on successful replacement.
 */
int replace_cmd_with_alias(info_t *info_struct)
{
	info_list *node;
	int count;
	char *ptr;

	for (count = 0; count < 10; count++)
	{
		node = node_starts_with(info_struct->alias, info_struct->argv[0], '=');
		if (!node)
		{
			return (0);
		}
		free(info_struct->argv[0]);
		ptr = _strchr(node->str, '=');
		if (!ptr)
		{
			return (0);
		}
		ptr = _strdup(ptr + 1);
		if (!ptr)
		{
			return (0);
		}
		info_struct->argv[0] = ptr;
	}
	return (1);
}

/**
 * replace_cmd_argv - Replace variables in the command arguments.
 * @info_struct: Pointer to the info_t structure.
 *
 * Description:
 * This function iterates through the command arguments in info_struct->argv
 * and replaces any variables (e.g., $?, $$, $VAR) with their corresponding
 * values. It checks for the special variables $? (status), $$ (process ID),
 * and user-defined environment variables.
 *
 * Return:
 * Always returns 0.
 */
int replace_cmd_argv(info_t *info_struct)
{
	int count = 0;
	info_list *node;

	for (count = 0; info_struct->argv[count]; count++)
	{
		if (info_struct->argv[count][0] != '$' || !info_struct->argv[count][1])
			continue;

		if (!_strcmp(info_struct->argv[count], "$?"))
		{
			replace_string(&(info_struct->argv[count]),
					_strdup(num_str_converter(info_struct->status, 10, 0)));
			continue;
		}
		if (!_strcmp(info_struct->argv[count], "$$"))
		{
			replace_string(&(info_struct->argv[count]),
					_strdup(num_str_converter(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(info_struct->env, &info_struct->argv[count][1], '=');
		if (node)
		{
			replace_string(&(info_struct->argv[count]),
					_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		replace_string(&info_struct->argv[count], _strdup(""));
	}
	return (0);
}


/**
 * replace_string - Replace a string with a new one.
 * @address_old: Pointer to the old string's address.
 * @new_string: New string to replace the old one.
 *
 * Description:
 * This function frees the memory occupied by the old string and replaces it
 * with the new string provided as input. The address of the old string is
 * updated to point to the new string.
 *
 * Return:
 * Always returns 1.
 */
int replace_string(char **address_old, char *new_string)
{
	free(*address_old);
	*address_old = new_string;
	return (1);
}

