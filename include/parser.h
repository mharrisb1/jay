#ifndef PARSER_H
#define PARSER_H

#include "json.h"
#include "lexer.h"

typedef struct {
  Lexer lexer;
  Token current_token;
} Parser;

void parser_init(Parser *parser, const char *input);
void parser_advance(Parser *parser);
void parser_error(const char *message);

JsonNode *parse_value(Parser *parser);
JsonNode *parse_object(Parser *parser);
JsonNode *parse_array(Parser *parser);

JsonNode *parse_json(const char *input);

#endif
