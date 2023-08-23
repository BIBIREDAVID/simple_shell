#include "main.h"

/**
 * add_node - Adds a new node to the beginning of a linked list.
 * @head: Pointer to the head of the linked list.
 * @str_char: String to be stored in the new node.
 * @num: Integer to be stored in the new node.
 *
 * Return: Pointer to the newly created node, or NULL if memory allocation
 * fails.
 *
 * Description: This function creates a new node and adds it to the
 * beginning of the linked list pointed to by 'head'. The node contains the
 * specified string 'str' and integer 'num'. If 'str' is NULL, only 'num' is
 * stored in the node.
 * The newly created node becomes the new head of the list.
 */
info_list *add_node(info_list **head, const char *str_char, int num)
{
	info_list *head_new;

	if (!head)
	{
		return (NULL);
	}
	head_new = malloc(sizeof(info_list));
	if (!head_new)
	{
		return (NULL);
	}
	_memset((void *)head_new, 0, sizeof(info_list));
	head_new->num = num;
	if (str_char)
	{
		head_new->str = _strdup(str_char);
		if (!head_new->str)
		{
			free(head_new);
			return (NULL);
		}
	}
	head_new->next = *head;
	*head = head_new;
	return (head_new);
}

/**
 * add_node_end - Adds a new node to the end of a linked list.
 * @head: Pointer to the head of the linked list.
 * @str_char: String to be stored in the new node.
 * @num: Integer to be stored in the new node.
 *
 * Return: Pointer to the newly created node, or NULL if memory allocation
 * fails.
 *
 * Description: This function creates a new node and adds it to the end of the
 * linked list pointed to by 'head'. The node contains the specified string
 * 'str' and integer 'num'. If 'str' is NULL, only 'num' is stored in the node.
 * If the linked list is empty, the new node becomes the head of the list.
 */
info_list *add_node_end(info_list **head, const char *str_char, int num)
{
	info_list *node_new, *node_current;

	if (!head)
	{
		return (NULL);
	}

	node_current = *head;
	node_new = malloc(sizeof(info_list));
	if (!node_new)
	{
		return (NULL);
	}
	_memset((void *)node_new, 0, sizeof(info_list));
	node_new->num = num;
	if (str_char)
	{
		node_new->str = _strdup(str_char);
		if (!node_new->str)
		{
			free(node_new);
			return (NULL);
		}
	}
	if (node_current)
	{
		while (node_current->next)
			node_current = node_current->next;
		node_current->next = node_new;
	}
	else
	{
		*head = node_new;
	}
	return (node_new);
}

/**
 * print_linked_list_str - Prints the strings in a linked list.
 * @head: Pointer to the head of the linked list.
 *
 * Return: Number of nodes in the linked list.
 *
 * Description: This function prints the strings stored in each node of the
 * linked list pointed to by 'h'. If a string is NULL, it prints "(nil)"
 * instead. It then moves to the next node and continues the process until
 * the end of the list is reached, counting the number of nodes along the way.
 */
size_t print_linked_list_str(const info_list *head)
{
	size_t count = 0;

	while (head)
	{
		_puts(head->str ? head->str : "(nil)");
		_puts("\n");
		head = head->next;
		count++;
	}
	return (count);
}


/**
 * delete_node_at_index - Deletes a node at a given index in a linked list.
 * @head: Double pointer to the head of the linked list.
 * @index: Index of the node to be deleted.
 *
 * Return: 1 if successful, 0 if failed.
 *
 * Description: This function deletes the node at the specified 'index' in
 * the linked list pointed to by '*head'. If the index is 0, the head node
 * is deleted. If the index is out of bounds or if the linked list is empty,
 * the function returns 0. If successful, the memory allocated for the
 * deleted node's string is freed and the node itself is removed from the
 * linked list.
 */
int delete_node_at_index(info_list **head, unsigned int index)
{
	info_list *node_current, *node_prev;
	unsigned int i = 0;

	if (!head || !*head)
	{
		return (0);
	}

	if (!index)
	{
		node_current = *head;
		*head = (*head)->next;
		free(node_current->str);
		free(node_current);
		return (1);
	}

	node_current = *head;
	while (node_current)
	{
		if (i == index)
		{
			node_prev->next = node_current->next;
			free(node_current->str);
			free(node_current);
			return (1);
		}
		i++;
		node_prev = node_current;
		node_current = node_current->next;
	}

	return (0);
}

/**
 * free_list - Frees the memory allocated for a linked list.
 * @head_ptr: Double pointer to the head of the linked list.
 *
 * Description: This function deallocates memory for each node in the linked
 * list and frees the strings stored in each node. After freeing all nodes,
 * the head of the linked list is set to NULL.
 */
void free_list(info_list **head_ptr)
{
	info_list *current_node, *node_next, *head_current;

	if (!head_ptr || !*head_ptr)
		return;

	head_current = *head_ptr;
	current_node = head_current;
	while (current_node)
	{
		node_next = current_node->next;
		free(current_node->str);
		free(current_node);
		current_node = node_next;
	}
	*head_ptr = NULL;
}
