#include "main.h"

/**
 * unset_hsh_alias - Unsets an alias from the alias list.
 * @info_struct: Pointer to the info structure.
 * @str: String in the format "alias=cmd" to unset the alias.
 *
 * Return: 1 on failure, 0 on success.
 *
 * Description: This function unsets an alias from the alias list of the info
 * structure. It extracts the alias name from the provided string, then deletes
 * the node with the matching name from the alias list. The function returns 1
 * on failure and 0 on success.
 */
int unset_hsh_alias(info_t *info_struct, char *str)
{
	char *p_flag, p_flag_temp;
	int ret_code;

	p_flag = _strchr(str, '=');
	if (!p_flag)
		return (1);

	p_flag_temp = *p_flag;
	*p_flag = 0;
	ret_code = delete_node_at_index(&(info_struct->alias),
			get_node_index(info_struct->alias,
				node_starts_with(info_struct->alias, str, -1)));
	*p_flag = p_flag_temp;

	return (ret_code);
}

/**
 * set_hsh_alias - Sets or updates an alias in the alias list.
 * @info_struct: Pointer to the info structure.
 * @str: String in the format "alias=cmd" to set or update the alias.
 *
 * Return: 1 on failure, 0 on success.
 *
 * Description: This function sets or updates an alias in the alias list of the
 * info structure. It extracts the alias name and command from the provided
 * string, then removes any existing alias with the same name. If the command
 * is empty, the function unsets the alias. Otherwise, it adds a new node to
 * the alias list.
 * The function returns 1 on failure and 0 on success.
 */
int set_hsh_alias(info_t *info_struct, char *str)
{
	char *p_flag;

	p_flag = _strchr(str, '=');
	if (!p_flag)
		return (1);

	if (!*++p_flag)
		return (unset_hsh_alias(info_struct, str));

	unset_hsh_alias(info_struct, str);
	return (add_node_end(&(info_struct->alias), str, 0) == NULL);
}


/**
 * print_hsh_alias - Prints the contents of an alias.
 * @node: Pointer to the alias node in the info_list.
 *
 * Return: 0 if the alias node is not NULL, 1 otherwise.
 *
 * Description: This function prints the contents of alias node in the format
 * "alias='cmd'". It extracts alias name and command from the node's string,
 * then prints the formatted alias. Function returns 0 if the alias node is
 * not NULL, or 1 if the node is NULL.
 */
int print_hsh_alias(info_list *node)
{
	char *p_flag = NULL, *hsh_alias = NULL;

	if (node)
	{
		p_flag = _strchr(node->str, '=');

		for (hsh_alias = node->str; hsh_alias <= p_flag; hsh_alias++)
			_putchar(*hsh_alias);

		_putchar('\'');
		_puts(p_flag + 1);
		_puts("'\n");

		return (0); /* Return 0 if the alias node is not NULL */
	}

	return (1); /* Return 1 if the alias node is NULL */
}



/**
 * hsh_alias - Handles the alias command.
 * @info_struct: Pointer to the info structure.
 *
 * Return: Always returns 0.
 *
 * Description: This function handles the alias command, used to define,
 * display, or modify command aliases. If called with no arguments, it displays
 * the list of existing aliases. If called with arguments in the format
 * "alias=cmd", it creates or updates an alias. If called with an argument not
 * in the format "alias=cmd", it attempts to display an alias matching the
 * provided name.
 * The function returns 0 to indicate success.
 */
int hsh_alias(info_t *info_struct)
{
	int count = 0;
	char *p_flag = NULL;
	info_list *node = NULL;

	/* Display existing aliases */
	if (info_struct->argc == 1)
	{
		node = info_struct->alias;
		while (node)
		{
			print_hsh_alias(node);
			node = node->next;
		}
		return (0);
	}

	/* Handle alias creation, update, or display */
	for (count = 1; info_struct->argv[count]; count++)
	{
		p_flag = _strchr(info_struct->argv[count], '=');
		if (p_flag)
			set_hsh_alias(info_struct, info_struct->argv[count]);
		else
			print_hsh_alias(node_starts_with(info_struct->alias,
						info_struct->argv[count], '='));
	}

	return (0); /* Always return 0 */
}
