#include "main.h"

/**
 * generate_history_filepath - Generates the path to the history file.
 * @info_struct: Pointer to the info_struct structure.
 *
 * Return: A dynamically allocated string containing the history file path,
 *         or NULL on failure.
 *
 * Description: This function generates the path to the history file using the
 * HOME environment variable and a predefined history file name. It retrieves
 * the HOME directory using the get_env_var function and then constructs the
 * complete path to the history file. The function returns a dynamically
 * allocated string containing the path, or NULL if memory allocation fails
 * or if the HOME environment variable is not set.
 */
char *generate_history_filepath(info_t *info_struct)
{
	char *buffer, *directory;

	directory = get_env_var(info_struct, "HOME=");
	if (!directory)
		return (NULL);

	buffer = malloc(sizeof(char) * (_strlen(directory) + _strlen(HIST_FILE) + 2));
	if (!buffer)
		return (NULL);

	buffer[0] = 0;
	_strcpy(buffer, directory);
	_strcat(buffer, "/");
	_strcat(buffer, HIST_FILE);

	return (buffer); /* Return the dynamically allocated history file path */
}


/**
 * write_history_to_file - Writes command history to a file.
 * @info_struct: Pointer to the info_struct structure.
 *
 * Return: 1 on success, -1 on failure.
 *
 * Description: This function writes the command history stored in the history
 * linked list of info_struct structure to a file. It creates or truncates the
 * history file specified by the history file path, then iterates through the
 * history list and writes each history entry to the file. The function returns
 * 1 on success and -1 on failure.
 */
int write_history_to_file(info_t *info_struct)
{
	info_list *node = NULL;
	ssize_t file_desc;
	char *file_name = generate_history_filepath(info_struct);

	if (!file_name)
		return (-1);

	file_desc = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(file_name);
	if (file_desc == -1)
		return (-1);

	for (node = info_struct->history; node; node = node->next)
	{
		print_string_file_desc(node->str, file_desc);
		print_char_file_desc('\n', file_desc);
	}

	/* Flush the buffer to the file */
	print_char_file_desc(BUF_FLUSH, file_desc);
	close(file_desc);

	return (1); /* Return 1 on success */
}

/**
 * read_command_history - Reads command history from a file into a linked list.
 * @info_struct: Pointer to the info_struct structure.
 *
 * Return: The number of history entries read, or 0 on failure.
 *
 * Description: This function reads command history from a file specified by
 * history file path stored in info_struct structure. It reads the file line
 * by line, populating the history linked list with each command. The function
 * returns the number of history entries read. If an error occurs, or if the
 * history file is empty or inaccessible, the function returns 0 to
 * indicate failure.
 */
int read_command_history(info_t *info_struct)
{
	struct stat env_stat;
	int count, last_count = 0, num_line_count = 0;
	ssize_t file_desc, read_len, file_size = 0;
	char *buffer = NULL, *filename = generate_history_filepath(info_struct);

	if (!filename)
		return (0);
	file_desc = open(filename, O_RDONLY);
	free(filename);
	if (file_desc == -1)
		return (0);
	if (!fstat(file_desc, &env_stat))
		file_size = env_stat.st_size;
	if (file_size < 2)
		return (0);
	buffer = malloc(sizeof(char) * (file_size + 1));
	if (!buffer)
		return (0);
	read_len = read(file_desc, buffer, file_size);
	buffer[file_size] = 0;
	if (read_len <= 0)
		return (free(buffer), 0);
	close(file_desc);
	for (count = 0; count < file_size; count++)
		if (buffer[count] == '\n')
		{
			buffer[count] = 0;
			add_to_history_list(info_struct, buffer + last_count, num_line_count++);
			last_count = count + 1;
		}
	if (last_count != count)
		add_to_history_list(info_struct, buffer + last_count, num_line_count++);
	free(buffer);
	info_struct->histcount = num_line_count;
	while (info_struct->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info_struct->history), 0);
	add_numbering_to_history(info_struct);
	return (info_struct->histcount);
}

/**
 * add_to_history_list - Builds a history entry and adds it to history list.
 * @info_struct: Pointer to the info_struct structure.
 * @buffer: The command to add to the history list.
 * @line_count: The line number of the command.
 *
 * Return: Always returns 0.
 *
 * Description: This function builds a history entry using the provided command
 * and line no, and adds it to history linked list in info_struct structure.
 * If history list is not empty, the new node is added to the end of the list.
 * The function returns 0 to indicate success.
 */
int add_to_history_list(info_t *info_struct, char *buffer, int line_count)
{
	info_list *node = NULL;

	if (info_struct->history)
		node = info_struct->history;

	add_node_end(&node, buffer, line_count); /* Add history entry to list */

	if (!info_struct->history)
		info_struct->history = node;

	return (0); /* Return 0 to indicate success */
}


/**
 * add_numbering_to_history - Renumbers the history entries in history list.
 * @info_struct: Pointer to the info_struct structure.
 *
 * Return: The updated number of history entries.
 *
 * Description: This function renumbers the history entries in history linked
 * list in info_struct structure. It assigns numbers starting from 0 to
 * each history entry. The function returns updated number of history entries
 * in the list.
 */
int add_numbering_to_history(info_t *info_struct)
{
	int count = 0;
	info_list *node = info_struct->history;

	while (node)
	{
		node->num = count++;
		node = node->next;
	}

	/* Update and return the number of history entries */
	return (info_struct->histcount = count);
}


