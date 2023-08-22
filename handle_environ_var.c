#include "main.h"

/**
 * display_env_var - Displays the list of environment variables.
 * @info_struct: Pointer to the info_struct structure.
 *
 * Return: Always returns 0.
 *
 * Description: This function displays the list of environment variables
 * stored in the info_struct structure. It uses the print_linked_list_str
 * function to print each environment variable string in the list. The
 * function always returns 0 to indicate success.
 */
int display_env_var(info_t *info_struct)
{
	/* Print the list of environment variables */
	print_linked_list_str(info_struct->env);
	return (0);				   /* Return 0 to indicate success */
}

/**
 * get_env_var - Retrieves the value of an environment variable.
 * @info_struct: Pointer to the info_struct structure.
 * @name: Name of the environment variable to retrieve.
 *
 * Return: Pointer to the value of the environment variable,
 * Return NULL if not found.
 *
 * Description: This function searches for an environment variable with the
 * specified name in environment list of info_struct structure. It returns a
 * pointer to the value of the environment variable if found, or NULL if the
 * variable is not present. The function iterates through the environment list
 * and uses the starts_with function to check if the current environment string
 * starts with the specified name. If a match is found, the function returns
 * a pointer to the value portion of the string.
 */
char *get_env_var(info_t *info_struct, const char *name)
{
	char *p;
	info_list *node = info_struct->env;

	while (node)
	{
		/* Check if environment string starts with the name */
		p = starts_with(node->str, name);

		if (p && *p)
		{
			/* Return the value portion of the environment string */
			return (p);
		}
		node = node->next;
	}

	return (NULL); /* Return NULL if the environment variable is not found */
}

/**
 * update_env_var - updates an environment variable based on command arguments.
 * @info_struct: Pointer to the info_struct structure.
 *
 * Return: 0 on success, 1 on failure.
 *
 * Description: This function sets or updates an environment variable based on
 * the command arguments provided in info_struct structure. If the number of
 * arguments is not exactly 3, an error message is printed, and the function
 * returns 1 to indicate failure. Otherwise, the _setenv function is used to
 * set or update the environment variable using the provided name and value.
 * The function returns 0 on success and 1 on failure.
 */
int update_env_var(info_t *info_struct)
{
	if (info_struct->argc != 3)
	{
		print_string("Incorrect number of arguments\n");
		return (1);
	}

	if (_setenv(info_struct, info_struct->argv[1], info_struct->argv[2]))
	{
		return (0);
	}

	return (1);
}

/**
 * clear_all_env_var - Unsets environment variables based on command arguments.
 * @info_struct: Pointer to the info_struct structure.
 *
 * Return: 0 on success, 1 on failure.
 *
 * Description: This function unsets environment variables based on the
 * command arguments provided in the info_struct structure. If the number of
 * arguments is too few (less than 2), an error message is printed and
 * the function returns 1 to indicate failure. Otherwise, for each argument,
 * the corresponding environment variable is unset using _unset_env function.
 * The function returns 0 to indicate success.
 */
int clear_all_env_var(info_t *info_struct)
{
	int count;

	if (info_struct->argc < 2)
	{
		print_string("Too few arguments.\n");
		return (1);
	}

	/* Unset each specified environment variable */
	for (count = 1; count < info_struct->argc; count++)
	{
		_unset_env(info_struct, info_struct->argv[count]);
	}

	return (0); /* Return 0 to indicate success */
}

/**
 * initialize_env_list - Populates an environment list in info_struct structure
 * @info_struct: Pointer to the info_struct structure.
 *
 * Return: Always returns 0.
 *
 * Description: This function populates the environment list in the info_struct
 * structure with the contents of the 'environ' array. For each element
 * in the 'environ' array, a new node is added to the end of the list,
 * containing the environment string. The function returns 0 to indicate
 * success.
 */
int initialize_env_list(info_t *info_struct)
{
	size_t i;
	info_list *node = NULL;

	/* Add each environment string to the list */
	for (i = 0; environ[i]; i++)
	{
		add_node_end(&node, environ[i], 0);
	}

	info_struct->env = node; /* Assign the list to the info_struct structure */

	return (0); /* Return 0 to indicate success */
}

