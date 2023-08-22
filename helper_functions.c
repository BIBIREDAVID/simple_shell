#include "main.h"

#include <sys/stat.h>

/**
 * is_exe_cmd - Check if a file is an executable command.
 * @info_struct: Pointer to the info_t structure.
 * @file_path: Path to the file to be checked.
 *
 * Description:
 * This function checks whether the specified file is a regular file and has
 * execute permissions, indicating that it can be executed as a command.
 *
 * Return:
 * Returns 1 if the file is an executable command, 0 otherwise.
 */
int is_exe_cmd(info_t *info_struct, char *file_path)
{
	struct stat stat_struct;

	(void)info_struct;
	if (!file_path || stat(file_path, &stat_struct) != 0)
	{
		return (0);
	}

	if (S_ISREG(stat_struct.st_mode) && (stat_struct.st_mode & S_IXUSR))
	{
		return (1);
	}

	return (0);
}


/**
 * duplicate_chars - Create a new null-terminated string from a substring.
 *
 * @str_path: The input string.
 * @start_index: The starting index (inclusive) of the substring.
 * @stop_index: The ending index (exclusive) of the substring.
 *
 * This function creates a new null-terminated string by copying characters
 * from the input string starting from the specified start index and ending at
 * the stop index. Colon characters (:) in the original string are omitted in
 * the copied substring.
 *
 * Return: A pointer to the copied substring (statically allocated).
 */
char *duplicate_chars(char *str_path, int start_index, int stop_index)
{
	static char buffer[1024];
	int count_i = 0, count_k = 0;

	for (count_i = start_index; count_i < stop_index; count_i++)
	{
		if (str_path[count_i] != ':')
		{
			buffer[count_k++] = str_path[count_i];
		}
	}
	buffer[count_k] = '\0';
	return (buffer);
}


/**
 * find_cmd_path - Find the full path of a command within a specified search
 * path.
 *
 * @info_struct: Pointer to the information structure.
 * @str_path: The search path containing colon-separated directories.
 * @cmd: The command name to search for.
 *
 * This function searches for the full path of a command by iterating through
 * the colon-separated directories in the search path. If the command starts
 * with "./" and is an executable in the current directory, it returns the
 * command itself. Otherwise, it looks for the command within each directory
 * in the search path.
 *
 * Return: A pointer to the full path of the command if found, otherwise NULL.
 */
char *find_cmd_path(info_t *info_struct, char *str_path, char *cmd)
{
	int count = 0, current_pos = 0;
	char *cmd_path;

	if (!str_path)
	{
		return (NULL);
	}
	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (is_exe_cmd(info_struct, cmd))
		{
			return (cmd);
		}
	}
	while (1)
	{
		if (!str_path[count] || str_path[count] == ':')
		{
			cmd_path = duplicate_chars(str_path, current_pos, count);
			if (!*cmd_path)
				_strcat(cmd_path, cmd);
			else
			{
				_strcat(cmd_path, "/");
				_strcat(cmd_path, cmd);
			}
			if (is_exe_cmd(info_struct, cmd_path))
				return (cmd_path);
			if (!str_path[count])
				break;
			current_pos = count;
		}
		count++;
	}
	return (NULL);
}

/**
 * strtow - Split a string into an array of words using delimiter characters.
 *
 * @str: The string to be split.
 * @delim: The delimiter characters.
 *
 * This function splits the input string into an array of words based on the
 * given delimiter characters. It counts the number of words in the string
 * and allocates memory for an array of pointers to the words. Then, it
 * populates the array with dynamically allocated memory for each word
 * extracted from the input string.
 *
 * Return: A dynamically allocated array of strings containing the words
 * from input string, or NULL if memory allocation fails or no words found.
 */
char **strtow(char *str, char *delim)
{
	int count_i, count_j, count_k, count_m, num_words = 0;
	char **s_arr;

	if (str == NULL || str[0] == 0)
		return (NULL);
	if (!delim)
		delim = " ";
	for (count_i = 0; str[count_i] != '\0'; count_i++)
		if (!is_delimiter(str[count_i], delim)
				&& (is_delimiter(str[count_i + 1], delim) || !str[count_i + 1]))
			num_words++;

	if (num_words == 0)
		return (NULL);
	s_arr = malloc((1 + num_words) * sizeof(char *));
	if (!s_arr)
		return (NULL);
	for (count_i = 0, count_j = 0; count_j < num_words; count_j++)
	{
		while (is_delimiter(str[count_i], delim))
			count_i++;
		count_k = 0;
		while (!is_delimiter(str[count_i + count_k], delim)
				&& str[count_i + count_k])
			count_k++;
		s_arr[count_j] = malloc((count_k + 1) * sizeof(char));
		if (!s_arr[count_j])
		{
			for (count_k = 0; count_k < count_j; count_k++)
				free(s_arr[count_k]);
			free(s_arr);
			return (NULL);
		}
		for (count_m = 0; count_m < count_k; count_m++)
			s_arr[count_j][count_m] = str[count_i++];
		s_arr[count_j][count_m] = 0;
	}
	s_arr[count_j] = NULL;
	return (s_arr);
}

/**
 * strtow2 - Split a string into an array of words using a delimiter character.
 *
 * @str: The string to be split.
 * @delim: The delimiter character.
 *
 * This function splits the input string into an array of words based on the
 * given delimiter character. It counts the number of words in the string and
 * allocates memory for an array of pointers to the words. Then, it populates
 * the array with dynamically allocated memory for each word extracted from the
 * input string.
 *
 * Return: A dynamically allocated array of strings containing the words from
 * the input string, or NULL if memory allocation fails or no words found.
 */
char **strtow2(char *str, char delim)
{
	int count_i, count_j, count_k, count_m, num_words = 0;
	char **s_arr;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (count_i = 0; str[count_i] != '\0'; count_i++)
		if ((str[count_i] != delim && str[count_i + 1] == delim) ||
				(str[count_i] != delim && !str[count_i + 1])
				|| str[count_i + 1] == delim)
			num_words++;
	if (num_words == 0)
		return (NULL);
	s_arr = malloc((1 + num_words) * sizeof(char *));
	if (!s_arr)
		return (NULL);
	for (count_i = 0, count_j = 0; count_j < num_words; count_j++)
	{
		while (str[count_i] == delim && str[count_i] != delim)
			count_i++;
		count_k = 0;
		while (str[count_i + count_k] != delim && str[count_i + count_k]
				&& str[count_i + count_k] != delim)
			count_k++;
		s_arr[count_j] = malloc((count_k + 1) * sizeof(char));
		if (!s_arr[count_j])
		{
			for (count_k = 0; count_k < count_j; count_k++)
				free(s_arr[count_k]);
			free(s_arr);
			return (NULL);
		}
		for (count_m = 0; count_m < count_k; count_m++)
			s_arr[count_j][count_m] = str[count_i++];
		s_arr[count_j][count_m] = 0;
	}
	s_arr[count_j] = NULL;
	return (s_arr);
}
