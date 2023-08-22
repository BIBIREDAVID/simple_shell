#include "main.h"

/**
 * parse_unsigned_Int - Converts a string to an integer.
 * @s: Pointer to the input string.
 *
 * Return: The integer value, or -1 on error.
 *
 * Description: This function converts the input string to an integer value.
 * It iterates through the characters of the string, checking if they are
 * digits (0-9). The result is accumulated by multiplying the current value
 * by 10 and adding the new digit. If the result exceeds INT_MAX, the function
 * returns -1 to indicate an error. If the string
 * contains non-digit characters, the function also returns -1.
 */
int parse_unsigned_Int(char *s)
{
	unsigned long int output = 0;
	int count = 0;

	if (*s == '+')
	{
		s++; /* Ignore leading '+' sign */
	}

	for (count = 0; s[count] != '\0'; count++)
	{
		if (s[count] >= '0' && s[count] <= '9')
		{
			output *= 10;
			output += (s[count] - '0');

			if (output > INT_MAX)
			{
				return (-1);
			}
		}
		else
		{
			return (-1);
		}
	}

	return (output);
}

/**
 * print_error - Prints an error message to standard error output.
 * @info_struct: Pointer to the information structure.
 * @str_error: Pointer to the error message string.
 *
 * Return: None.
 *
 * Description: This function prints an error message to standard error output.
 * It combines various pieces of information from the 'info_struct' to create
 * a formatted error message. The message includes the file name, line count,
 * command name, and the provided error string ('estr'). The function is
 * typically used to display error messages associated with command execution
 * or other program errors.
 */
void print_error(info_t *info_struct, char *str_error)
{
	print_string(info_struct->fname);
	print_string(": ");
	print_decimal(info_struct->line_count, STDERR_FILENO);
	print_string(": ");
	print_string(info_struct->argv[0]);
	print_string(": ");
	print_string(str_error);
}

/**
 * num_str_converter - Converts a number to a string representation.
 * @num: The number to be converted.
 * @base: The base for the conversion (e.g., 10 for decimal).
 * @flags: Flags to control conversion behavior.
 *
 * Return: Pointer to the converted string.
 *
 * Description: This function converts a number to its string representation
 * using the specified base. The 'flags' parameter can be used to control
 * the conversion behavior, such as handling unsigned numbers or using
 * lowercase hexadecimal characters. The function performs the conversion by
 * iteratively dividing the number by the base and building the string
 * representation.
 * The resulting string is stored in the 'buffer' array, and a pointer to the
 * beginning of the string is returned.
 */
char *num_str_converter(long int num, int base, int flags)
{
	static char *array_char;
	static char buffer[50];
	char sign_char = 0;
	char *ptr_str;
	unsigned long num_to_convert = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		num_to_convert = -num;
		sign_char = '-';
	}

	array_char = flags & CONVERT_LOWERCASE
		? "0123456789abcdef" : "0123456789ABCDEF";
	ptr_str = &buffer[49];
	*ptr_str = '\0';

	do {
		*--ptr_str = array_char[num_to_convert % base];
		num_to_convert /= base;
	} while (num_to_convert != 0);

	if (sign_char)
		*--ptr_str = sign_char;

	return (ptr_str);
}

/**
 * remove_comments - Removes comments from a string.
 * @buf: Pointer to the string containing comments.
 *
 * Return: None.
 *
 * Description: This function removes comments from the provided string.
 * It iterates through the characters of the string and looks for '#' char
 * that are either at the beginning of the line or preceded by a space.
 * When a '#' is found meeting these conditions, the rest of the line is
 * truncated, effectively removing the comment. The function modifies
 * the original string in place.
 */
void remove_comments(char *buf)
{
	int count;

	for (count = 0; buf[count] != '\0'; count++)
	{
		if (buf[count] == '#' && (!count || buf[count - 1] == ' '))
		{
			buf[count] = '\0'; /* Truncate the string at the comment */
			break;
		}
	}
}
