#include "main.h"

/**
 * _memset - Fill memory with a constant byte.
 * @mem_ptr: Pointer to the memory to be filled.
 * @value: The value to be set (interpreted as an unsigned char).
 * @num_bytes: Number of bytes to be set to the value.
 *
 * Description: This function sets each byte of the memory region pointed to
 * by @s to the specified value @b, for @n number of bytes.
 *
 * Return: Pointer to the memory region @s.
 */
char *_memset(char *mem_ptr, char value, unsigned int num_bytes)
{
	unsigned int count;

	for (count = 0; count < num_bytes; count++)
	{
		mem_ptr[count] = value;
	}
	return (mem_ptr);
}


/**
 * free_ptr_array - Free an array of pointers and the array itself.
 * @array_ptr: Double pointer to the array of pointers to be freed.
 *
 * Description: This function frees the memory occupied by each element in the
 * array of pointers @pp and then frees the memory occupied by the array @pp.
 * The array @pp should be null-terminated.
 */
void free_ptr_array(char **array_ptr)
{
	char **array_ptr_temp = array_ptr;

	if (!array_ptr)
	{
		return;
	}

	while (*array_ptr)
	{
		free(*array_ptr++);
	}
	free(array_ptr_temp);
}


/**
 * _realloc - Reallocate memory block with new size.
 * @prev_ptr: Pointer to the previously allocated memory.
 * @size_old: Size of the previously allocated memory.
 * @size_new: Size of the new memory block.
 *
 * Description: This function reallocates a memory block previously allocated
 * with malloc, changing its size to @size_new bytes. If @prev_ptr is NULL,
 * then the function behaves like malloc. If @size_new is 0, then the function
 * behaves like free. If @size_old and @size_new are the same, the function
 * returns @prev_ptr unchanged. Otherwise, it allocates a new block of memory
 * with the requested size, copies the contents from the previous block, and
 * frees the old block.
 *
 * Return: On success, returns a pointer to the newly allocated memory block.
 * On failure, returns NULL.
 */
void *_realloc(void *prev_ptr, unsigned int size_old, unsigned int size_new)
{
	char *p;

	if (!prev_ptr)
	{
		return (malloc(size_new));
	}

	if (!size_new)
	{
		free(prev_ptr);
		return (NULL);
	}

	if (size_new == size_old)
		return (prev_ptr);

	p = malloc(size_new);
	if (!p)
	{
		return (NULL);
	}

	size_old = size_old < size_new ? size_old : size_new;
	while (size_old--)
	{
		p[size_old] = ((char *)prev_ptr)[size_old];
	}
	free(prev_ptr);
	return (p);
}

/**
 * free_ptr - frees a pointer and NULLs the address
 * @d_ptr: address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int free_ptr(void **d_ptr)
{
	if (d_ptr && *d_ptr)
	{
		free(*d_ptr);
		*d_ptr = NULL;
		return (1);
	}
	return (0);
}
