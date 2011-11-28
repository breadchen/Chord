#include <stdio.h>
#include <string.h>

#include "identifier.h"

static int is_zero(const identifier id_in);

identifier_element* identifier_add(identifier id_added, const identifier id_in)
{
	int counter;
	int carry = 0;
	int buf;

	for (counter = 0; counter < ID_BYTE_LEN; counter++)
	{
		buf = carry + id_in[counter] + id_added[counter];
		if (buf > BYTE_MAX)
		{ // have carry
			id_added[counter] = buf - BYTE_MAX;
			carry = 1;
		}
		else
		{
			id_added[counter] = buf;
			carry = 0;
		}
	} // end of for

	return id_added;
}

identifier_element* identifier_minus(identifier id_minused, const identifier id_in)
{
	int counter;
	int borrow = 0;
	int buf;

	for (counter = 0; counter < ID_BYTE_LEN; counter++)
	{
		buf = (int)id_minused[counter] - id_in[counter] - borrow;
		if (buf < 0)
		{
			id_minused[counter] = buf + BYTE_MAX;
			borrow = 1;
		}
		else
		{
			id_minused[counter] = buf;
			borrow = 0;
		}
	} // end of for

	return id_minused;
}

identifier_element* identifier_clear(identifier_element value, identifier id_out)
{
	int counter;

	for (counter = 0; counter < ID_BYTE_LEN; counter++)
		id_out[counter] = value;
	
	return id_out;
}

int identifier_compare(const identifier id_l, const identifier id_r)
{
	int counter;

	for (counter = 0; counter < ID_BYTE_LEN; counter++)
	{
		if (id_l[counter] > id_r[counter])
			return 1;
		else if (id_l[counter < id_r[counter]])
			return -1;
	}
	return 0;
}

int identifier_is_between(const identifier id_in,
						  const identifier id_start,
						  const identifier id_end,
						  int b_start, int b_end)
{
	identifier buf_in;	
	identifier interval;
	int result = 0;
	
	memcpy(buf_in, id_in, ID_BYTE_LEN);
	memcpy(interval, id_end, ID_BYTE_LEN);

	identifier_minus(buf_in, id_start);
	if (is_zero(id_in)) return b_start;

	identifier_minus(interval, id_start);

	switch (identifier_compare(buf_in, interval))
	{
		case -1: result = 1; break;
		case 0: result = b_end; break;
		case 1: result = 0; break;
		default: break;
	}
	return result;
}

void identifier_print(identifier id_in)
{
	int i;

	for (i = ID_BYTE_LEN - 1; i >= 0; i--)
		printf("%02x", id_in[i]);
	printf("\n");
}

static int is_zero(const identifier id_in)
{
	int i;
	
	for(i = 0; i < ID_BYTE_LEN; i++)
		if (id_in[i] != 0) return 0;
	
	return 1;
}
