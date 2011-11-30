#ifndef _KEY_H
#define _KEY_H

#include "identifier.h"
#include <arpa/inet.h>

struct key
{
//	unsigned char id[ID_BYTE_LEN];
	identifier id;
	struct sockaddr_in addr;
};

/* generate the key of a string */
struct key* key_generate_by_str(const char* str_in, struct key* key_out);

/* generate the key of ip and port, 
 * actually it use key_generate_by_str with input a string like "192.168.1.1:8000" 
 */
struct key* key_generate_by_addr(struct sockaddr_in addr, struct key* key_out);

/* generate the key of current time and a random value */
struct key* key_generate_random(struct key* key_out);

/* compare key_l and key_r, 
 * if key_l > key_r, return 1
 * if key_l = key_r, return 0
 * if key_l < key_r, return -1
 */
int key_compare(const struct key* key_l, const struct key* key_r);

/* add key_in to key_added */
struct key* key_add(struct key* key_added, const struct key* key_in);

/* minus key_minused by key_in */
struct key* key_minus(struct key* key_minused, const struct key* key_in);

/* generate the key of pow(2, exponent) 
 * make sure exponent is smaller than ID_BIT_LEN
 */
struct key* key_generate_power(int exponent, struct key* key_out);

/* example: if key_is_between(key_in, key_start, key_end, 0, 1); returns 0
 * 	  it means key_in is not in interval (key_start, key_end]
 * 	  if key_is_between(key_in, key_start, key_end, 0, 0); returns 1
 * 	  it means key_in is in interval (key_start, key_end)
 */
int key_is_between(const struct key* key_in, 
				   const struct key* key_start,
				   const struct key* key_end,
				   int b_start, int b_end);

/* clear key_out with value */
struct key* key_clear(unsigned char value, struct key* key_out);

/* print key.id from ID_BYTE_LEN-1 to 0 */
void key_print(struct key* key_in);

#endif
