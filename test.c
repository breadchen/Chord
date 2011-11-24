#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "key.h"

void main()
{
	struct sockaddr_in ip;
	unsigned char ibuf[1024];
	struct key obuf;
	//SHA_CTX content;
	int i;

	ip.sin_family = AF_INET;
	ip.sin_port = 8000;

	scanf("%s", ibuf);
	inet_aton(ibuf, (struct in_addr*)&ip.sin_addr);

	//SHA1_Init(&content);
	//SHA1_Update(&content, ibuf, strlen(ibuf));
	//SHA1_Final(obuf, &content);
	key_generate_by_addr(ip, &obuf);

	for (i = 0; i < 20; i++)
		printf("%02x", obuf.id[i]);
	printf("\n");

	key_generate_random(&obuf);

	for (i = 0; i < 20; i++)
		printf("%02x", obuf.id[i]);
	printf("\n");
}
