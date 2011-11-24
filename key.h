#ifndef _KEY_H
#define _KEY_H

#define ID_BYTE_LEN 20
#define ID_BIT_LEN 160

struct key
{
	unsigned char id[ID_BYTE_LEN];
	struct sockaddr_in addr;
};

/* generate the key of a string */
struct key* key_generate_by_str(const char* str_in, struct key* key_out);

/* generate the key of ip and port, 
 * actually it use key_generate_by_str with input a string like "192.168.1.1:8000" */
struct key* key_generate_by_addr(struct sockaddr_in addr, struct key* key_out);

/* generate the key of current time and a random value */
struct key* key_generate_random(struct key* key_out);

#endif
