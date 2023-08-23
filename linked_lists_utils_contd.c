#include "main.h"

/**
 * linked_list_len - Calculates the length of a linked list.
 * @head: Pointer to the head of the linked list.
 *
 * Description: This function counts the number of nodes in the linked list
 * and returns the count.
 *
 * Return: The number of nodes in the linked list.
 */
size_t linked_list_len(const info_list *head)
{
	size_t count = 0;

	while (head)
	{
		head = head->next;
		count++;
	}
	return (count);
}


/**
 * linked_list_to_strings - Converts a linked list of strings into an array of
 * strings.
 * @head: Pointer to the head of the linked list.
 *
 * Description: This function creates an array of strings by iterating through
 * the linked list and copying the content of each node's string into the
 * array.
 *
 * Return: A dynamically allocated array of strings, or NULL on failure.
 */
char **linked_list_to_strings(info_list *head)
{
	char **strs_memory;
	char *str;
	info_list *node_current = head;
	size_t count = linked_list_len(head), j;

	if (!head || !count)
	{
		return (NULL);
	}

	strs_memory = malloc(sizeof(char *) * (count + 1));
	if (!strs_memory)
	{
		return (NULL);
	}

	for (count = 0; node_current; node_current = node_current->next, count++)
	{
		str = malloc(_strlen(node_current->str) + 1);
		if (!str)
		{
			for (j = 0; j < count; j++)
				free(strs_memory[j]);
			free(strs_memory);
			return (NULL);
		}

		str = _strcpy(str, node_current->str);
		strs_memory[count] = str;
	}

	strs_memory[count] = NULL;
	return (strs_memory);
}

/**
 * print_linked_list - Prints the contents of a linked list.
 * @head: Pointer to the head of the linked list.
 *
 * Description: This function prints the node numbers, followed by a colon
 * and a space, and then prints the content of each node's string. If the
 * string is NULL, it prints "(nil)".
 *
 * Return: The number of nodes in the linked list.
 */
size_t print_linked_list(const info_list *head)
{
	size_t count = 0;

	while (head)
	{
		/* Convert and print node number */
		_puts(num_str_converter(head->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(head->str ? head->str : "(nil)"); /* Print the node's string */
		_puts("\n");
		head = head->next;
		count++;
	}
	return (count);
}

/**
 * node_starts_with - Searches for a node with a string starting with a prefix.
 * @node: Pointer to the head of the linked list.
 * @prefix: The prefix to search for.
 * @ch: Optional character to match (-1 means no character check).
 *
 * Description: This function searches through a linked list of info_list nodes
 * to find the first node whose string content starts with the given prefix and
 * optionally matches the specified character (if provided). It returns a
 * pointer to the found node or NULL if no matching node is found.
 *
 * Return: A pointer to the found node, or NULL if not found.
 */
info_list *node_starts_with(info_list *node, char *prefix, char ch)
{
	char *start_ptr = NULL;

	while (node)
	{
		start_ptr = starts_with(node->str, prefix);
		if (start_ptr && ((ch == -1) || (*start_ptr == ch)))
		{
			return (node);
		}
		node = node->next;
	}
	return (NULL);
}


/**
 * get_node_index - Get the index of a node in a linked list.
 * @head: Pointer to the head of the linked list.
 * @node: Pointer to the node whose index is to be calculated.
 *
 * Description: This function calculates the index of a specific node within
 * a linked list. It starts from the head of the list and iterates through
 * each node until it finds the desired node. The index of the node is then
 * returned. If the node is not found in the list, it returns -1.
 *
 * Return: The index of the node, or -1 if the node is not found.
 */
ssize_t get_node_index(info_list *head, info_list *node)
{
	size_t count = 0;

	while (head)
	{
		if (head == node)
		{
			return (count);
		}
		head = head->next;
		count++;
	}
	return (-1);
}

