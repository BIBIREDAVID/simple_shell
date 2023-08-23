#include "main.h"

/**
 * print_string - Prints a string to the standard error stream.
 * @str: The string to be printed.
 *
 * Description: This function prints each character of the given string
 * to the standard error stream until the null terminator is encountered.
 * If the input string is NULL, the function returns immediately.
 */
void print_string(char *str)
{
	int count = 0;

	if (!str)
	{
		return;
	}

	while (str[count] != '\0')
	{
		print_char(str[count]); /* Print each character */
		count++;
	}
}

/**
 * print_char - Prints a character to the standard error stream.
 * @c: The character to be printed.
 *
 * Return: Always returns 1.
 *
 * Description: This function prints the given character to the standard
 * error stream. The characters are buffered until the buffer is full
 * or a special character (BUF_FLUSH) is encountered, at which point
 * the buffer is flushed to the standard error stream. The function
 * always returns 1 to indicate success.
 */
int print_char(char c)
{
	static char buffer[WRITE_BUF_SIZE];
	static int count;

	if (c == BUF_FLUSH || count >= WRITE_BUF_SIZE)
	{
		write(2, buffer, count); /* Flush the buffer */
		count = 0;
	}

	if (c != BUF_FLUSH)
		buffer[count++] = c; /* Add character to buffer */

	return (1); /* Return 1 to indicate success */
}

/**
 * print_char_file_desc - Writes a character to a specified file descriptor.
 * @c: The character to be written.
 * @file_desc: The file descriptor to write to.
 *
 * Return: Always returns 1.
 *
 * Description: This function writes the given character to the specified
 * file descriptor. The characters are buffered until the buffer is full
 * or a special character (BUF_FLUSH) is encountered, at which point
 * the buffer is flushed to the specified file descriptor. The function
 * always returns 1 to indicate success.
 */
int print_char_file_desc(char c, int file_desc)
{
	static int count;
	static char buffer[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || count >= WRITE_BUF_SIZE)
	{
		write(file_desc, buffer, count); /* Flush the buffer*/
		count = 0;
	}

	if (c != BUF_FLUSH)
	{
		buffer[count++] = c; /* Add character to buffer */
	}

	return (1); /* Return 1 to indicate success */
}

/**
 * print_string_file_desc - Writes a string to a specified file descriptor.
 * @str: The string to be written.
 * @file_desc: The file descriptor to write to.
 *
 * Return: The number of characters written.
 *
 * Description: This function writes each character of the given string
 * to the specified file descriptor using the specified print function.
 * If the input string is NULL, the function returns 0. The function
 * returns the total number of characters written to the file descriptor.
 */
int print_string_file_desc(char *str, int file_desc)
{
	int count = 0;

	if (!str)
	{
		return (0);
	}

	while (*str)
	{
		/* Write each character*/
		count += print_char_file_desc(*str++, file_desc);
	}

	return (count); /* Return the total number of characters written */
}


/**
 * print_decimal - Prints an integer to a file descriptor.
 * @input: The integer to be printed.
 * @file_desc: The file descriptor to write to.
 *
 * Return: The number of characters printed.
 *
 * Description: This function prints an integer to the specified file
 * descriptor. If the file descriptor is STDERR_FILENO, it uses the
 * 'print_char' function for output. The function handles negative
 * numbers by printing the minus sign and converting the integer to its
 * absolute value. It then iterates through the digits of the integer and
 * print each digit character. The function returns the number of
 * characters printed.
 */
int print_decimal(int input, int file_desc)
{
	int (*__putchar)(char) = _putchar;
	int i_iter, count_dec = 0;
	unsigned int _abs_dec_, current_dec;

	if (file_desc == STDERR_FILENO)
	{
		__putchar = print_char;
	}

	if (input < 0)
	{
		_abs_dec_ = -input;
		__putchar('-');
		count_dec++;
	}
	else
	{
		_abs_dec_ = input;
	}

	current_dec = _abs_dec_;
	for (i_iter = 1000000000; i_iter > 1; i_iter /= 10)
	{
		if (_abs_dec_ / i_iter)
		{
			__putchar('0' + current_dec / i_iter);
			count_dec++;
		}
		current_dec %= i_iter;
	}

	__putchar('0' + current_dec);
	count_dec++;

	return (count_dec);
}
