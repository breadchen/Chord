#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#include <openssl/sha.h>
#include "key.h"

static char* addr_to_str(const struct sockaddr_in addr, char* str_out);
static int is_zero(struct key* key_in);

/*
 * Function: use ip and port to generate SHA1 code
 *
 */
struct key* key_generate_by_addr(const struct sockaddr_in addr, struct key* key_out)
{
	char buffer[256];
	return key_generate_by_str(addr_to_str(addr, buffer), key_out);
}

/*
 * Function: use string to generate SHA1 code
 *
 */
struct key* key_generate_by_str(const char* str_in, struct key* key_out)
{
	SHA1((unsigned char*)str_in, strlen(str_in), key_out->id);	
	return key_out; 
}

/*
 * Function: use current time to generate random SHA1 code
 *
 */
struct key* key_generate_random(struct key* key_out)
{
	time_t now;
	struct tm* now_local;
	char* str_time;
	char buffer[1024];

	time(&now);
	now_local = localtime(&now); 
	str_time = asctime(now_local);
	printf("time: %s", str_time);

	srand((unsigned int)now);
	sprintf(buffer, "%s%d", str_time, rand());

	return key_generate_by_str(buffer, key_out);
}

/* 
 * Function: generate the key of pow(2, exponent)
 *
 */
struct key* key_generate_power(int exponent, struct key* key_out)
{
	int byte_index, bit_index;

	if (exponent > ID_BIT_LEN)
		return key_out;
	
	byte_index = exponent / 8;
	bit_index = exponent % 8;
	key_clear(0, key_out);
	key_out->id[byte_index] = 1 << bit_index;

	return key_out;
}

int key_compare(const struct key* key_l, const struct key* key_r)
{/*
	int counter;

	for (counter = 0; counter < ID_BYTE_LEN; counter++)
	{
		if (key_l->id[counter] > key_r->id[counter])
			return 1;
		else if (key_l->id[counter < key_r->id[counter]])
			return -1;
	}
	return 0;
	*/
	return identifier_compare(key_l->id, key_r->id);
}

/*
 * Function: add key_in->id to key_added->id 
 *    each byte in result has moded BYTE_MAX
 *
 */
struct key* key_add(struct key* key_added, const struct key* key_in)
{/*
	int counter;
	int carry = 0;
	int buf;

	for (counter = 0; counter < ID_BYTE_LEN; counter++)
	{
		buf = carry + key_in->id[counter] + key_added->id[counter];
		if (buf > BYTE_MAX)
		{ // have carry
			key_added->id[counter] = buf - BYTE_MAX;
			carry = 1;
		}
		else
		{
			key_added->id[counter] = buf;
			carry = 0;
		}
	} // end of for
*/
	identifier_add(key_added->id, key_in->id);
	return key_added;
}

/*
 * Function: minus key_minused->id by key_in->id
 * 	  each byte in result has moded BYTE_MAX
 *
 */
struct key* key_minus(struct key* key_minused, const struct key* key_in)
{/*
	int counter;
	int borrow = 0;
	int buf;

	for (counter = 0; counter < ID_BYTE_LEN; counter++)
	{
		buf = (int)key_minused->id[counter] - key_in->id[counter] - borrow;
		if (buf < 0)
		{
			key_minused->id[counter] = buf + BYTE_MAX;
			borrow = 1;
		}
		else
		{
			key_minused->id[counter] = buf;
			borrow = 0;
		}
	} // end of for
*/
	identifier_minus(key_minused->id, key_in->id);
	return key_minused;
}

int key_is_between(const struct key* key_in, 
				   const struct key* key_start,
				   const struct key* key_end,
				   int b_start, int b_end)
{/*
	struct key buf_in;	
	struct key interval;
	int result = 0;
	
	memcpy(buf_in.id, key_in->id, ID_BYTE_LEN);
	memcpy(interval.id, key_end->id, ID_BYTE_LEN);

	key_minus(&buf_in, key_start);
	if (is_zero(&buf_in)) return b_start;

	key_minus(&interval, key_start);

	switch (key_compare(&buf_in, &interval))
	{
		case -1: result = 1; break;
		case 0: result = b_end; break;
		case 1: result = 0; break;
		default: break;
	}
	return result;
*/
	return identifier_is_between(key_in->id, key_start->id, key_end->id, 
			b_start, b_end);
}

/* clear key_out with value */
struct key* key_clear(identifier_element value, struct key* key_out)
{/*
	int counter;

	for (counter = 0; counter < ID_BYTE_LEN; counter++)
		key_out->id[counter] = value;
*/
	identifier_clear(value, key_out->id);
	return key_out;
}

/* print key.id from ID_BYTE_LEN-1 to 0 */
void key_print(struct key* key_in)
{/*
	int i;

	for (i = ID_BYTE_LEN - 1; i >= 0; i--)
		printf("%02x", key_in->id[i]);
	printf("\n");
	*/
	identifier_print(key_in->id);
}

static int is_zero(struct key* key_in)
{
	int i;
	
	for(i = 0; i < ID_BYTE_LEN; i++)
		if (key_in->id[i] != 0) return 0;
	
	return 1;
}

/*
 * Function: convert ip and port to string formated like "192.168.1.1:8000"
 * 
 */
static char* addr_to_str(const struct sockaddr_in addr, char* str_out)
{
	char* buffer;

	buffer = inet_ntoa((struct in_addr)addr.sin_addr);
	sprintf(str_out, "%s:%d", buffer, addr.sin_port);

	return str_out;
}
