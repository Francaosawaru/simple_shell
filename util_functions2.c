#include "shell.h"

/**
 * my_memchr - searches the memory for a character
 * @src_void: string to search
 * @c: the character to find
 * @length: of the string
 *
 * Return: nil
 */
void *my_memchr(const void *src_void, int c, size_t length)
{
	const unsigned char *src = (const unsigned char *)src_void;

	while (length-- > 0)
	{
		if (*src == c)
			return ((void *)src);
		src++;
	}
	return (NULL);
}

/**
 * my_strtok - tokenizes a string
 * @str: strings to tokenize
 * @delim: iters to use
 * Return: a token
 */
char *my_strtok(char *str, const char *delim)
{
	static char *s;
	char *ret = NULL;

	if (str != NULL)
	{
		s = str;
	}
	if (s != NULL && my_strlen(s))
	{
		const size_t dlen = my_strlen((char *)delim);

		while (*s && my_memchr(delim, *s, dlen) != NULL)
		{
			++s;
		}

		if (*s)
		{
			ret = s;

			while (*s)
			{
				if (my_memchr(delim, *s, dlen) != NULL)
				{
					break;
				}
				else
					++s;
			}
			if (*s)
			{
				s[0] = 0;
				++s;
			}
		}
	}
	return (ret);
}

/**
 * my_memcpy - copies data to another pointer with casts
 * @dest: dest to paste data
 * @src: source to copy data from
 * @size: length of data to copy
 * Return: nothing
 */
void my_memcpy(void *dest, const void *src, unsigned int size)
{
	char *ptr = (char *)src;
	char *new = (char *)dest;
	unsigned int j;

	for (j = 0; j < size; j++)
		new[j] = ptr[j];
}

/**
 * my_realloc - reallocates memory
 * @ptr: pointer previously allocated with malloc
 * @old_size: the previous poniter size
 * @new_size: new size to allocate
 *
 * Return: pointer to new memory
 */
void *my_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *nptr;
	unsigned int j;

	if (new_size == old_size)
		return (ptr);

	if (ptr == NULL)
	{
		ptr = malloc(new_size);
		return (ptr);
	}

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	nptr = malloc(new_size);
	if (nptr == NULL)
		return (NULL);

	for (j = 0; j < new_size && j < old_size; j++)
		;

	memcpy(nptr, ptr, j);
	free(ptr);

	return (nptr);
}

/**
 * my_isdigit - check if a string contains digits only
 * @nstr: string to be checked
 * Return: 1 if digit, 0 if not digit
 */
int my_isdigit(char *nstr)
{
	int j;

	for (j = 0; nstr[j] != '\0' ; j++)
	{
		if (nstr[j] < 48 || nstr[j] > 57)
			return (0);
	}

	return (1);
}
