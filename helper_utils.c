#include "main.h"

/**
 * is_interactive - Check if the shell is running in is_interactive mode.
 *
 * @info_struct: Pointer to the information structure.
 *
 * This function checks whether the shell is running in an is_interactive mode
 * by determining if the standard input is a terminal and if the read file
 * descriptor is less than or equal to 2 (indicating standard input, output,
 * or error).
 *
 * Return: 1 if the shell is running in is_interactive mode, 0 otherwise.
 */
int is_interactive(info_t *info_struct)
{
	return (isatty(STDIN_FILENO) && info_struct->readfd <= 2);
}


/**
 * is_delimiter - Check if a character is a delimiter.
 *
 * @c: The character to check.
 * @delimiter: The string containing delimiters.
 *
 * This function checks whether the given character is present in the provided
 * delimiter string, indicating whether it is a delimiter or not.
 *
 * Return: 1 if the character is a delimiter, 0 otherwise.
 */
int is_delimiter(char c, char *delimiter)
{
	while (*delimiter)
	{
		if (*delimiter++ == c)
		{
			return (1);
		}
	}

	return (0);
}


/**
 * is_alpha - Check if a character is an alphabetical character.
 *
 * @ch: The character to check.
 *
 * This function checks whether the given character is an alphabetical
 * character (either uppercase or lowercase).
 *
 * Return: 1 if the character is an alphabetical character, 0 otherwise.
 */
int is_alpha(int ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

/**
 * convert_str_to_int - Convert a string to an integer.
 *
 * @input_str: The input string to be converted.
 *
 * This function converts the input string to an integer. It handles negative
 * numbers and stops converting when non-digit characters are encountered.
 *
 * Return: The integer representation of the input string.
 */
int convert_str_to_int(char *input_str)
{
	unsigned int result = 0;
	int count, str_sign = 1, str_flag = 0, str_output;

	for (count = 0; input_str[count] != '\0' && str_flag != 2; count++)
	{
		if (input_str[count] == '-')
		{
			str_sign *= -1;
		}

		if (input_str[count] >= '0' && input_str[count] <= '9')
		{
			str_flag = 1;
			result *= 10;
			result += (input_str[count] - '0');
		}
		else if (str_flag == 1)
		{
			str_flag = 2;
		}
	}

	if (str_sign == -1)
	{
		str_output = -result;
	}
	else
	{
		str_output = result;
	}

	return (str_output);
}
