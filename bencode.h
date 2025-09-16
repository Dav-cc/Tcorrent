#ifndef __BENCODE__
#define __BENCODE__

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Tor_value Tor_value;
typedef struct Tor_dict_entry Tor_dict_entry;



typedef enum{
	TOR_INT = 0,
	TOR_STRING,
	TOR_LIST,
	TOR_DICT
}Tor_token;

typedef struct Tor_dict_entry{
	char *key;
	Tor_value *value; 
}Tor_dict_entry;

struct Tor_value{
	Tor_token type;
	union{
		int64_t integer;
		struct{
			int64_t length;
			const char* data;
		}string;
		struct{
			Tor_value **elements;
			ssize_t count;
			ssize_t capacity;
		}list;
		struct{
			Tor_dict_entry* entries;
			ssize_t count;
			ssize_t capacity;
		}dict;
	};
};


typedef struct {
	const char *data;
	size_t len;
	size_t pos;
}Tor_parser;


/* function decelerations */
  
Tor_value* decode_bencode(const char* bn_data, size_t data_len);
Tor_value* Tor_parse_value(Tor_parser* parser);
Tor_value* parse_integer(Tor_parser* parser);
/* static inline int next_char(Tor_parser* parser); */
/* static inline int peek_char(Tor_parser* parser); */
Tor_value* parse_integer(Tor_parser *parser);
Tor_value* parse_string(Tor_parser *parser);
Tor_value* parse_list(Tor_parser *parser);
Tor_value* parse_dict(Tor_parser *parser);
#endif
