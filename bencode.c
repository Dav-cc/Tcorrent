#include "bencode.h"

int is_digit(char c){
    return c>='0' && c<='9';
}

static inline int peek_char(Tor_parser* parser){
    return (parser->pos < parser->len) ? parser->data[parser->pos] : '\0';
}

static inline int next_char(Tor_parser* parser){
    return (parser->pos < parser->len)? parser->data[parser->pos++] : '\0';
}


Tor_value* decode_bencode(const char* bn_data, size_t data_len){
    if((bn_data == NULL)&&(data_len <=0)) return NULL;
    Tor_parser parser = {
        .data = bn_data,
        .len = data_len,
        .pos = 0
    };
    
    Tor_value* result = Tor_parse_value(&parser);

    return result;
}

Tor_value* Tor_parse_value(Tor_parser* parser){
    if(parser->data == NULL || parser->len == 0) return NULL;
    
    char c = peek_char(parser);
    switch(c){
        case 'i': 
             return parse_integer(parser); 

        case 'l':
            return parse_list(parser);
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9': case '0':
            return parse_string(parser);
        default :
            return NULL;
    }
}


Tor_value* parse_integer(Tor_parser *parser){
    char c;
    int is_negative = 0;
    int64_t value = 0;
    if(next_char(parser)!= 'i') return NULL;
    
    c = peek_char(parser);

    if(c == '-'){
        is_negative = 1;
        next_char(parser);
    }

    while(( c = peek_char(parser)) != 'e' && (c != '\0')){
        if(!is_digit(c)) return NULL;
        
        value = value * 10 + (c - '0');
        c = next_char(parser);
    }

    Tor_value* result = malloc(sizeof(Tor_value));
    result->type = TOR_INT;
    result->integer = is_negative ? -value : value;

    printf("\n[Debug]: value is %ld\n", result->integer);
    return result;

}

Tor_value* parse_string(Tor_parser *parser){
    int64_t length = 0;
    char c;

    while((c = peek_char(parser))!= ':' &&  (c != '\0')){
        if(!is_digit(c)) return NULL;

        length = length * 10 +(c - '0');
        next_char(parser);
    }
    if(next_char(parser)!= ':')return NULL;

    Tor_value* result = malloc(sizeof(Tor_value));
    result->string.data  = malloc(length+1);
    result->type = TOR_STRING;
    result->string.length = length;

    memcpy((void*)result->string.data, parser->data + parser->pos , length);

    parser->pos += length;
    return result;
}



Tor_value* parse_list(Tor_parser* parser){
    char c;
    if((c = next_char(parser)) != 'l') return NULL;

    Tor_value* result = malloc(sizeof(Tor_value));
    result->type = TOR_LIST;
    result->list.capacity = 0;
    result->list.count = 0;
    result->list.elements = NULL;
    
    while((c = peek_char(parser)) != 'e' && c != '\0'){
        if(result->list.count >= result->list.capacity){
            size_t new_cap = (result->list.count) ? (result->list.count)*2 : 8 ;
            Tor_value** new_result = realloc(result->list.elements, new_cap * sizeof(Tor_value*));
            if(new_result == NULL) return NULL;
            result->list.capacity = new_cap;
            result->list.elements = new_result;
        }

        Tor_value* element = Tor_parse_value(parser);
        result->list.elements[result->list.count++] = element;

        if(peek_char(parser) =='e')
            next_char(parser);
    }
    return result;
    
}
















