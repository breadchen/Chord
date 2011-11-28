#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "key.h"

void main()
{
	unsigned char ibuf[1024];
	struct key obuf;
	struct key add_in;
	struct key random;
	int exponent = 0;
	//SHA_CTX content;

	obuf.addr.sin_family = AF_INET;
	obuf.addr.sin_port = 8000;

	scanf("%s", ibuf);
	inet_aton(ibuf, (struct in_addr*)&obuf.addr.sin_addr);

	//SHA1_Init(&content);
	//SHA1_Update(&content, ibuf, strlen(ibuf));
	//SHA1_Final(obuf, &content);
	key_generate_by_addr(obuf.addr, &obuf);
	key_print(&obuf);

	key_generate_random(&obuf);
	key_print(&obuf);

	scanf("\n%d", &exponent);
	key_generate_power(exponent, &add_in);
	key_print(&add_in);

	key_add(&obuf, &add_in);
	key_print(&obuf);

	key_generate_random(&random);
	key_print(&random);
	if (key_is_between(&add_in, &obuf, &random, 0,0))
		printf("in\n");
	else
		printf("not in\n");
}
