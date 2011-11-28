#ifndef _IDENTIFIER_H
#define _IDENTIFIER_H

#define ID_BYTE_LEN 20
#define ID_BIT_LEN 160
#define BYTE_MAX 255

typedef unsigned char identifier_element;
typedef identifier_element identifier[ID_BYTE_LEN];

identifier_element* identifier_add(identifier id_added, const identifier id_in);

identifier_element* identifier_minus(identifier id_minused, const identifier id_in);

identifier_element* identifier_clear(identifier_element value, identifier id_out);

int identifier_compare(const identifier id_l, const identifier id_r);

int identifier_is_between(const identifier id_in,
						  const identifier id_start,
						  const identifier id_end,
						  int b_start, int b_end);

void identifier_print(identifier id_in);

#endif
