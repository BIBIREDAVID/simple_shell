#include "main.h"

/**
 * _strlen - Computes the length of a string.
 * @s: Pointer to the input string.
 *
 * Return: The length of the string.
 *
 * Description: This function calculates the length of a string by iterating
 * through each character until the null terminator is encountered. If the
 * input string is NULL, the function returns 0.
 */
int _strlen(char *s)
{
	int count = 0;

	if (!s)
		return (0);

	while (*s++)
		count++;

	return (count);
}


/**
 * _strcmp - Compares two strings.
 * @s1: Pointer to the first string.
 * @s2: Pointer to the second string.
 *
 * Return: An integer less than, equal to, or greater than 0, depending on
 * whether the first string is less than, equal to, or greater than the second
 * string.
 *
 * Description: This function compares two strings character by character.
 * It returns an integer value indicating their lexicographical relationship.
 * A value less than 0 means the first string is lexicographically less than
 * the second, a value of 0 indicates equality, and a value greater than 0
 * means the first string is lexicographically greater than the second string.
 */
int _strcmp(char *str1, char *str2)
{
	while (*str1 && *str2)
	{
		if (*str1 != *str2)
		{
			return (*str1 - *str2);
		}

		str1++;
		str2++;
	}

	if (*str1 == *str2)
	{
		return (0);
	}
	else
	{
		return (*str1 < *str2 ? -1 : 1);
	}
}

/**
 * starts_with - Checks if a string starts with another string.
 * @haystack: Pointer to the main string.
 * @needle: Pointer to the string to be checked.
 *
 * Return: Pointer to the starting position in 'haystack' if 'needle' is found
 * at the beginning, or NULL if not found.
 *
 * Description: This function checks if the 'needle' string starts with the
 * characters in the 'haystack' string. It iterates through both strings until
 * the end of 'needle' is reached. If the characters match, it continues to
 * compare the rest of 'needle'. If the comparison succeeds, it returns a
 * pointer to the position in 'haystack' where the 'needle' starts; otherwise,
 * it returns NULL.
 */
char *starts_with(const char *haystack, const char *needle)
{
	while (*needle)
	{
		if (*needle++ != *haystack++)
		{
			return (NULL);
		}
	}

	return ((char *)haystack);
}

/**
 * _strcat - Concatenates two strings.
 * @dest: Pointer to the destination string.
 * @src: Pointer to the source string.
 *
 * Return: Pointer to the destination string after concatenation.
 *
 * Description: This function appends the characters of the 'src' string to
 * the end of the 'dest' string. It iterates through the 'dest' string to find
 * its end, and then appends the characters of the 'src' string. The resulting
 * concatenated string is null-terminated. The function returns a pointer to
 * the 'dest' string after the concatenation is complete.
 */
char *_strcat(char *dest, char *src)
{
	char *concat_str = dest;

	while (*dest)
	{
		dest++;
	}

	while (*src)
	{
		*dest++ = *src++;
	}

	*dest = '\0'; /* Null-terminate the concatenated string */
	return (concat_str);
}

/**
 * _strchr - Locates the first occurrence of a character in a string.
 * @s: Pointer to the string.
 * @ch: The character to search for.
 *
 * Return: Pointer to the first occurrence of 'ch' in 's', or NULL if not
 * found.
 *
 * Description: This function searches for the first occurrence of the
 * character 'ch' in the string pointed to by 's'. It iterates through the
 * string and returns a pointer to the first occurrence of 'ch' if found,
 * or NULL if 'ch' is not present in the string.
 */
char *_strchr(char *s, char ch)
{
	do {
		if (*s == ch)
			return (s);
	} while (*s++ != '\0');

	return (NULL);
}

