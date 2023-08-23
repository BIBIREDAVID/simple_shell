#include "main.h"

/**
 * input_buffer - Read input buffer from the user.
 * @info_struct: Pointer to the info_t structure.
 * @buffer: Pointer to a pointer that will hold the input buffer.
 * @len: Pointer to the size of the input buffer.
 *
 * Description: This function reads input from user, fills the input buffer,
 * and handles various scenarios such as removing trailing newlines, removing
 * comments, and adding the input to the history list. If no input is left in
 * the buffer, it will read a new line from the user and perform the necessary
 * operations on it.
 *
 * Return: On success, returns the number of characters read. On failure,
 * returns -1.
 */
ssize_t input_buffer(info_t *info_struct, char **buffer, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, sigint_handler);
#if USE_GETLINE
		r = getline(buffer, &len_p, stdin);
#else
		r = get_stdin_line(info_struct, buffer, &len_p);
#endif
		if (r > 0)
		{
			if ((*buffer)[r - 1] == '\n')
			{
				(*buffer)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			info_struct->linecount_flag = 1;
			remove_comments(*buffer);
			add_to_history_list(info_struct, *buffer, info_struct->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info_struct->cmd_buf = buffer;
			}
		}
	}
	return (r);
}

/**
 * get_input - Get input from the user and handle command chaining.
 * @info_struct: Pointer to the info_t structure.
 *
 * Description: This function reads input from the user and handles command
 * chaining by detecting and processing semicolons. It also maintains a buffer
 * for command chaining and iterates through the buffer to find individual
 * commands separated by semicolons.
 *
 * Return: On success, returns the number of characters read. On failure,
 * returns -1.
 */
ssize_t get_input(info_t *info_struct)
{
	static char *buffer; /* the ';' command chain buffer */
	static size_t count_i, count_j, len;
	ssize_t buffer_len = 0;
	char **buffer_ptr = &(info_struct->arg), *ptr;

	_putchar(BUF_FLUSH);
	buffer_len = input_buffer(info_struct, &buffer, &len);
	if (buffer_len == -1) /* EOF */
		return (-1);
	if (len) /* we have commands left in the chain buffer */
	{
		count_j = count_i;		 /* init iterator to current buf position */
		ptr = buffer + count_i; /* get pointer for return */

		check_cmd_chain(info_struct, buffer, &count_j, count_i, len);
		while (count_j < len) /* iterate to semicolon or end */
		{
			if (is_chain_delimiter(info_struct, buffer, &count_j))
			{
				break;
			}
			count_j++;
		}

		count_i = count_j + 1;	  /* increment past nulled ';'' */
		if (count_i >= len) /* reached end of buffer? */
		{
			count_i = len = 0; /* reset position and length */
			info_struct->cmd_buf_type = CMD_NORM;
		}

		*buffer_ptr = ptr;		/* pass pointer to current command position */
		return (_strlen(ptr)); /* return length of current command */
	}

	*buffer_ptr = buffer; /* else not chain, pass back buffer frm get_stdin_line() */
	return (buffer_len);	  /* return length of buffer from get_stdin_line() */
}

/**
 * read_buffer - Read data into a buffer from a file descriptor.
 * @info_struct: Pointer to the info_t structure.
 * @buffer: Buffer to store the read data.
 * @size: Pointer to the size of the buffer.
 *
 * Description: This function reads data into a buffer from the specified file
 * descriptor. If the buffer is already filled, no data is read. If successful,
 * the function updates the size to the number of bytes read.
 *
 * Return: On success, returns the number of bytes read. On failure or if the
 * buffer is already filled, returns 0 or -1.
 */
ssize_t read_buffer(info_t *info_struct, char *buffer, size_t *size)
{
	ssize_t read_r = 0;

	if (*size)
	{
		return (0);
	}
	read_r = read(info_struct->readfd, buffer, READ_BUF_SIZE);

	if (read_r >= 0)
	{
		*size = read_r;
	}
	return (read_r);
}


/**
 * get_stdin_line - Read a line from standard input.
 * @info_struct: Pointer to the info_t structure.
 * @ptr_address: Pointer to the address of the buffer containing the line.
 * @length: Pointer to the length of the buffer.
 *
 * Description: This function reads a line from standard input into a buffer,
 * expanding the buffer as needed. It maintains state to track the current
 * position within the buffer for efficient reading across multiple calls.
 *
 * Return: On success, returns the length of the line (including newline
 * character). Returns -1 on failure or end-of-file.
 */
int get_stdin_line(info_t *info_struct, char **ptr_address, size_t *length)
{
	size_t count_k;
	ssize_t read = 0, s_len = 0;
	char *ptr = NULL, *ptr_new = NULL, *c_ptr;
	static char buffer[READ_BUF_SIZE];
	static size_t count_i, len;

	ptr = *ptr_address;
	if (ptr && length)
		s_len = *length;
	if (count_i == len)
		count_i = len = 0;

	read = read_buffer(info_struct, buffer, &len);
	if (read == -1 || (read == 0 && len == 0))
		return (-1);

	c_ptr = _strchr(buffer + count_i, '\n');
	count_k = c_ptr ? 1 + (unsigned int)(c_ptr - buffer) : len;
	ptr_new = _realloc(ptr, s_len, s_len ? s_len + count_k : count_k + 1);
	if (!ptr_new) /* MALLOC FAILURE! */
		return (ptr ? free(ptr), -1 : -1);

	if (s_len)
		_strncat(ptr_new, buffer + count_i, count_k - count_i);
	else
		_strncpy(ptr_new, buffer + count_i, count_k - count_i + 1);

	s_len += count_k - count_i;
	count_i = count_k;
	ptr = ptr_new;

	if (length)
		*length = s_len;
	*ptr_address = ptr;
	return (s_len);
}

/**
 * sigint_handler - Signal handler for SIGINT (Ctrl+C).
 * @signal_num: The signal number (unused).
 *
 * Description:
 * This function is called when the user presses Ctrl+C.
 * It prints a newline and the shell prompt to start a new line.
 *
 * Return: None.
 */
void sigint_handler(__attribute__((unused)) int signal_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

