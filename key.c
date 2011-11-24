#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#include <openssl/sha.h>
#include "key.h"

static char* addr_to_str(const struct sockaddr_in addr, char* str_out);

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
