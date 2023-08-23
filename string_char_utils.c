#include "main.h"

/**
 * _strcpy - Copies a string to another string.
 * @dest: Pointer to the destination string.
 * @src: Pointer to the source string.
 *
 * Return: Pointer to the destination string after copying.
 *
 * Description: This function copies the characters of the 'src' string to the
 * 'dest'string. It iterates through the 'src' string, copying each character
 * to the 'dest' string until the null terminator is encountered. The resulting
 * copied string is null-terminated. The function returns a pointer to the
 * 'dest' string after copying is complete.
 */
char *_strcpy(char *dest, char *src)
{
	int count = 0;

	if (dest == src || src == 0)
		return (dest);

	while (src[count])
	{
		dest[count] = src[count];
		count++;
	}

	dest[count] = '\0'; /* Null-terminate the copied string */
	return (dest);
}


/**
 * _strdup - Duplicates a string in memory.
 * @str: Pointer to the input string.
 *
 * Return: Pointer to the newly allocated duplicated string, or NULL on
 * failure.
 *
 * Description: This function duplicates the input string by allocating
 * memory for a new string and copying the characters from the input string
 * into it. The resulting duplicated string is null-terminated. The function
 * returns a pointer to the duplicated string on success, or NULL if memory
 * allocation fails or if the input string is NULL.
 */
char *_strdup(const char *str)
{
	int str_len = 0;
	char *duplicate;

	if (str == NULL)
	{
		return (NULL);
	}

	while (*str++)
	{
		str_len++;
	}

	duplicate = malloc(sizeof(char) * (str_len + 1));

	if (!duplicate)
	{
		return (NULL);
	}

	for (str_len++; str_len--;)
	{
		duplicate[str_len] = *--str;
	}

	return (duplicate);
}


/**
 * _strncat - Concatenates two strings up to a specified length.
 * @dest_char: Pointer to the destination string.
 * @src_char: Pointer to the source string.
 * @n: Maximum number of characters to concatenate from 'src_char'.
 *
 * Return: Pointer to the concatenated string 'dest_char'.
 *
 * Description: This function appends a copy of the string pointed to by
 * 'src_char' to the end of the string pointed to by 'dest_char', up to 'n'
 * characters. The function then null-terminates the result. It returns a
 * pointer to the resulting concatenated string 'dest_char'.
 */
char *_strncat(char *dest_char, char *src_char, int n)
{
	int count_i, count_j;
	char *dest_char_temp = dest_char;

	count_i = 0;
	count_j = 0;
	while (dest_char[count_i] != '\0')
		count_i++;
	while (src_char[count_j] != '\0' && count_j < n)
	{
		dest_char[count_i] = src_char[count_j];
		count_i++;
		count_j++;
	}
	if (count_j < n)
		dest_char[count_i] = '\0';
	return (dest_char_temp);
}

/**
 * _puts - Prints a string to the standard output.
 * @str: Pointer to the input string.
 *
 * Return: None.
 *
 * Description: This function prints the characters of the input string to the
 * standard output until the null terminator is encountered. If the input
 * string is NULL, the function does nothing.
 */
void _puts(char *str)
{
	int count = 0;

	if (!str)
	{
		return;
	}

	while (str[count] != '\0')
	{
		_putchar(str[count]);
		count++;
	}
}

/**
 * _putchar - Writes a character to the standard output.
 * @c: Character to be written.
 *
 * Return: 1 on success, 0 on failure.
 *
 * Description: This function writes a character to the standard output.
 * It uses an internal buffer to improve efficiency. If the buffer is full or
 * if the provided character is BUF_FLUSH (flush signal), the function writes
 * the buffer to the standard output. The function returns 1 on success and
 * 0 on failure.
 */
int _putchar(char c)
{
	static int count;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || count >= WRITE_BUF_SIZE)
	{
		write(1, buf, count);
		count = 0;
	}

	if (c != BUF_FLUSH)
		buf[count++] = c;

	return (1);
}

